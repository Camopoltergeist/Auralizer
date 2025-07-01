#pragma once
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "miniaudio.h"

#include "AppState.hpp"

void load_audio_file(AppState* app_state) {
	ma_sound* sound = new ma_sound();

	ma_result result;

	result = ma_sound_init_from_file(app_state->audio_engine, app_state->audio_file_path.c_str(), 0, nullptr, nullptr, sound);

	if (result != MA_SUCCESS) {
		SDL_Log("Failed to load %s", app_state->audio_file_path.c_str());
		return;
	}

	if (app_state->sound != nullptr) {
		ma_sound_stop(app_state->sound);
		ma_sound_uninit(app_state->sound);
	}

	app_state->sound = sound;
	ma_sound_start(sound);
}

void file_dialog_callback(void* userdata, const char* const* file_list, int filter) {
	AppState* state = static_cast<AppState*>(userdata);

	if (file_list == nullptr || *file_list == nullptr) {
		return;
	}

	state->audio_file_path = *file_list;
	state->is_audio_file_selected = true;

	load_audio_file(state);
}

void draw_gui(AppState* app_state) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello ImGui!");

	if (app_state->is_audio_file_selected) {
		ImGui::Text(app_state->audio_file_path.c_str());
	}
	else {
		ImGui::Text("No file selected");
	}

	ImGui::SameLine();

	const SDL_DialogFileFilter file_filters[] = {
		{ "Wave files (*.wav)", "wav" }
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

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}