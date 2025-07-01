#pragma once
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "AppState.hpp"

void load_audio_file(AppState* app_state) {
	SDL_AudioSpec file_spec;
	SDL_zero(file_spec);

	Uint8* audio_data = nullptr;
	Uint32 audio_length = 0;

	if (!SDL_LoadWAV(app_state->audio_file_path.c_str(), &file_spec, &audio_data, &audio_length)) {
		SDL_Log("Failed to load test.wav: %s", SDL_GetError());
		return;
	}

	SDL_AudioStream* stream = SDL_CreateAudioStream(&file_spec, &app_state->audio_device_spec);

	if (stream == nullptr) {
		SDL_Log("Failed to create audio stream: %s", SDL_GetError());
		return;
	}

	SDL_DestroyAudioStream(app_state->audio_stream);
	SDL_free(app_state->audio_data);

	app_state->audio_stream = stream;
	app_state->audio_data = audio_data;
	app_state->audio_data_length = audio_length;

	SDL_BindAudioStream(app_state->audio_device, stream);
	SDL_PutAudioStreamData(stream, audio_data, audio_length);
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
		SDL_SetAudioDeviceGain(app_state->audio_device, volume);
		app_state->audio_volume = volume;
	}

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}