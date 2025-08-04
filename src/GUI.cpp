#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <codecvt>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include "miniaudio.h"

#include "AppState.hpp"

void load_audio_file(AppState* app_state, const std::string& file_path) {
	auto* sound = new ma_sound();

	// Very deprecated, but no better option exists in C++ at the moment.
	// Needed as SDL3's file dialog returns UTF8 string, but miniaudio expects either ASCII or UTF-16 on Windows.
	// Just fucking shoot me.
	// No idea how cross platform this piece of code is.
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	const std::wstring wide_path = converter.from_bytes(file_path);

	ma_result result = ma_sound_init_from_file_w(app_state->audio_engine->get_engine(), wide_path.c_str(), MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT | MA_SOUND_FLAG_DECODE, nullptr, nullptr, sound);

	if (result != MA_SUCCESS) {
		SDL_Log("Failed to load %s", file_path.c_str());
		return;
	}

	if (app_state->sound != nullptr) {
		ma_sound_stop(app_state->sound);
		ma_sound_uninit(app_state->sound);
	}

	const std::string audio_file_name = std::filesystem::path(file_path).stem().string();

	app_state->sound = sound;
	app_state->audio_file_path = file_path;
	app_state->audio_file_name = audio_file_name;
	app_state->is_audio_file_selected = true;

	ma_node_attach_output_bus(sound, 0, app_state->analysis_node.get(), 0);
	ma_sound_set_looping(sound, true);
	ma_sound_start(sound);

	app_state->is_playing = true;
}

void file_dialog_callback(void* userdata, const char* const* file_list, int filter) {
	auto* state = static_cast<AppState*>(userdata);

	if (file_list == nullptr || *file_list == nullptr) {
		return;
	}

	load_audio_file(state, *file_list);
}

void generate_time_text(std::stringstream& time_stream, float cursor_pos, float sound_length) {
	const int total_minutes = static_cast<int>(std::floor(sound_length / 60.f));
	const float total_seconds = std::fmod(sound_length, 60.f);

	const int current_minutes = static_cast<int>(std::floor(cursor_pos / 60.f));
	const float current_seconds = std::fmod(cursor_pos, 60.f);

	time_stream << std::setprecision(2) << std::fixed
		<< std::setfill('0') << std::setw(2) << current_minutes
		<< ':'
		<< std::setfill('0') << std::setw(5) << current_seconds
		<< " - "
		<< std::setfill('0') << std::setw(2) << total_minutes
		<< ':'
		<< std::setfill('0') << std::setw(5) << total_seconds;
}

void audio_file_mode(AppState* app_state, const bool mode_changed)
{
	if(mode_changed) {
		if(app_state->is_playing) {
			ma_sound_start(app_state->sound);
		}

		app_state->capture_device = nullptr;
	}

	if (app_state->is_audio_file_selected) {
		ImGui::Text(app_state->audio_file_name.c_str());
	}
	else {
		ImGui::Text("No file selected");
	}

	constexpr SDL_DialogFileFilter file_filters[] = {
		{ "Audio files (*.wav, *.flac, *.mp3)", "wav;flac;mp3" }
	};

	if (ImGui::Button("Select Audio File")) {
		SDL_ShowOpenFileDialog(file_dialog_callback, app_state, app_state->main_window.get_window_ptr(), file_filters, 1, nullptr, false);
	}

	float sound_length = 0.00001f;
	float cursor = 0.f;

	if (app_state->sound != nullptr) {
		ma_sound_get_length_in_seconds(app_state->sound, &sound_length);
		ma_sound_get_cursor_in_seconds(app_state->sound, &cursor);
	}

	std::stringstream time_stream;

	generate_time_text(time_stream, cursor, sound_length);

	ImGui::SliderFloat("##time_slider", &cursor, 0.f, sound_length, time_stream.str().c_str());

	if (app_state->sound == nullptr) {
		ImGui::BeginDisabled();
	}

	const char* play_button_string = app_state->is_playing ? "Pause" : "Play";

	if (ImGui::Button(play_button_string)) {
		if (app_state->is_playing) {
			ma_sound_stop(app_state->sound);
		}
		else {
			ma_sound_start(app_state->sound);
		}

		app_state->is_playing = !app_state->is_playing;
	}

	if (app_state->sound == nullptr) {
		ImGui::EndDisabled();
	}

	if (ImGui::SliderFloat("Volume", &app_state->audio_volume, 0.f, 1.f)) {
		app_state->audio_engine->set_volume(app_state->audio_volume);
	}
}

void microphone_mode(AppState* app_state, const bool mode_changed)
{
	if(mode_changed) {
		ma_sound_stop(app_state->sound);
	}

	bool changed = ImGui::Combo("Select device", &app_state->capture_device_selection, app_state->audio_context->get_capture_device_names().data(), static_cast<int>(app_state->audio_context->get_capture_device_names().size()));
	ImGui::SameLine();

	if(ImGui::Button("Refresh")) {
		app_state->audio_context->update_device_list();
	}
	else if(changed || !app_state->capture_device) {
		if(!app_state->audio_context->get_capture_devices().empty()) {
			app_state->capture_device = CaptureDevice::create(*app_state->audio_context, app_state->audio_context->get_capture_devices().at(app_state->capture_device_selection).id);
		}
	}
}

void draw_gui(AppState* app_state) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, nullptr, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);

	if (app_state->is_imgui_window_open) {
		ImGui::Begin("Inspector", &app_state->is_imgui_window_open);
		ImGui::SeparatorText("Inspector (F10 to toggle)");
		ImGui::Text("Frame time: %.3f ms",  app_state->last_frame_time.count() * 1000.f);

		const AudioMode last_mode = app_state->audio_mode;

		ImGui::RadioButton("Audio File", reinterpret_cast<int*>(&app_state->audio_mode), static_cast<int>(AudioMode::AudioFile));
		ImGui::SameLine();
		ImGui::RadioButton("Microphone", reinterpret_cast<int*>(&app_state->audio_mode), static_cast<int>(AudioMode::Microphone));

		const bool mode_changed = app_state->audio_mode != last_mode;

		switch (app_state->audio_mode) {
			case AudioMode::AudioFile: {
				audio_file_mode(app_state, mode_changed);
				break;
			}

			case AudioMode::Microphone: {
				microphone_mode(app_state, mode_changed);
				break;
			}
		}

		ImGui::SeparatorText("Analyzer");
		ImGui::DragFloatRange2("dB range", &app_state->analyser.min_db, &app_state->analyser.max_db, 0.1f, -120.f, 0.f, "Min: %.1f dB", "Max: %.1f dB");

		ImGui::SeparatorText("Shader");

		if(app_state->graphics.fragment_shader.has_value()) {
			ImGui::Text(app_state->fragment_shader_file_path.c_str());
		}
		else {
			ImGui::Text("No shader loaded");
		}

		ImGui::SameLine();

		if(ImGui::Button("Reload Shader")) {
			app_state->graphics.load_shader(app_state->fragment_shader_file_path);
		}

		ImGui::End();
	}
	
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
