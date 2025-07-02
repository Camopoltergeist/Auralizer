#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <codecvt>
#include <cmath>
#include <sstream>
#include <iomanip>

#include "miniaudio.h"

#include "AppState.hpp"

void load_audio_file(AppState* app_state, const std::string& file_path) {
	ma_sound* sound = new ma_sound();

	ma_result result;

	// Very deprecated, but no better option exists in C++ at the moment.
	// Needed as SDL3's file dialog returns UTF8 string, but miniaudio expects either ASCII or UTF-16 on Windows.
	// Just fucking shoot me.
	// No idea how cross platform this piece of code is.
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide_path = converter.from_bytes(file_path);

	result = ma_sound_init_from_file_w(app_state->audio_engine, wide_path.c_str(), 0, nullptr, nullptr, sound);

	if (result != MA_SUCCESS) {
		SDL_Log("Failed to load %s", file_path.c_str());
		return;
	}

	if (app_state->sound != nullptr) {
		ma_sound_stop(app_state->sound);
		ma_sound_uninit(app_state->sound);
	}

	app_state->sound = sound;
	app_state->audio_file_path = file_path;
	app_state->is_audio_file_selected = true;

	ma_sound_set_looping(sound, true);
	ma_sound_start(sound);
}

void file_dialog_callback(void* userdata, const char* const* file_list, int filter) {
	AppState* state = static_cast<AppState*>(userdata);

	if (file_list == nullptr || *file_list == nullptr) {
		return;
	}

	load_audio_file(state, *file_list);
}

void generate_time_text(std::stringstream& time_stream, float cursor_pos, float sound_length) {
	int total_minutes = std::floor(sound_length / 60.f);
	float total_seconds = std::fmod(sound_length, 60.f);

	int current_minutes = std::floor(cursor_pos / 60.f);
	float current_seconds = std::fmod(cursor_pos, 60.f);

	time_stream << std::setprecision(2) << std::fixed
		<< std::setfill('0') << std::setw(2) << current_minutes
		<< ':'
		<< std::setfill('0') << std::setw(5) << current_seconds
		<< " - "
		<< std::setfill('0') << std::setw(2) << total_minutes
		<< ':'
		<< std::setfill('0') << std::setw(5) << total_seconds;
}

void draw_gui(AppState* app_state) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello ImGui!", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (app_state->is_audio_file_selected) {
		ImGui::Text(app_state->audio_file_path.c_str());
	}
	else {
		ImGui::Text("No file selected");
	}

	ImGui::SameLine();

	const SDL_DialogFileFilter file_filters[] = {
		{ "Audio files (*.wav, *.flac, *.mp3)", "wav;flac;mp3" }
	};

	if (ImGui::Button("Select")) {
		SDL_ShowOpenFileDialog(file_dialog_callback, app_state, app_state->main_window, file_filters, 1, nullptr, false);
	}

	float volume = app_state->audio_volume;
	ImGui::SliderFloat("Volume", &volume, 0.f, 1.f);

	if (volume != app_state->audio_volume) {
		ma_engine_set_volume(app_state->audio_engine, volume);
		app_state->audio_volume = volume;
	}

	float sound_length = 0.f;
	float cursor = 0.f;

	if (app_state->sound != nullptr) {
		ma_sound_get_length_in_seconds(app_state->sound, &sound_length);
		ma_sound_get_cursor_in_seconds(app_state->sound, &cursor);

		std::stringstream time_stream;

		generate_time_text(time_stream, cursor, sound_length);

		ImGui::SliderFloat("##time_slider", &cursor, 0.f, sound_length, time_stream.str().c_str());
	}

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}