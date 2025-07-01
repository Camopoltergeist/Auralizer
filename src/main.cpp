#include <iostream>

#include <glad.h>
#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "AppState.hpp"
#include "init.hpp"


SDL_AppResult SDL_AppInit(void** app_state, int argc, char** argv) {
	AppState* new_state = new AppState();
	*app_state = new_state;

	if (!init_video_subsystem()) {
		return SDL_APP_FAILURE;
	}

	if (!init_window(new_state)) {
		return SDL_APP_FAILURE;
	}

	if (!init_opengl(new_state)) {
		return SDL_APP_FAILURE;
	}

	init_imgui(new_state);

	if (!init_audio_subsystem()) {
		return SDL_APP_FAILURE;
	}

	if (!init_audio_device(new_state)) {
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

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

SDL_AppResult SDL_AppIterate(void* app_state) {
	AppState* state = static_cast<AppState*>(app_state);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(state->vertex_array_object);
	glBindProgramPipeline(state->pipeline);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello ImGui!");

	if (state->is_audio_file_selected) {
		ImGui::Text(state->audio_file_path.c_str());
	}
	else {
		ImGui::Text("No file selected");
	}

	ImGui::SameLine();

	const SDL_DialogFileFilter file_filters[] = {
		{ "Wave files (*.wav)", "wav" }
	};

	if (ImGui::Button("Select")) {
		SDL_ShowOpenFileDialog(file_dialog_callback, app_state, state->main_window, file_filters, 1, nullptr, false);
	}

	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(state->main_window);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* app_state, SDL_Event* event) {
	ImGui_ImplSDL3_ProcessEvent(event);

	switch (event->type)
	{
		case SDL_EVENT_QUIT: {
			return SDL_APP_SUCCESS;
		}

		case SDL_EVENT_WINDOW_RESIZED: {
			Sint32 w = event->window.data1;
			Sint32 h = event->window.data2;

			glViewport(0, 0, w, h);

			return SDL_APP_CONTINUE;
		}

		default:
			break;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* app_state, SDL_AppResult result) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	delete app_state;
}