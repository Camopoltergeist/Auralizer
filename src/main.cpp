#include <iostream>

#include <glad.h>
#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <fftw3.h>

#include "AppState.hpp"
#include "init.hpp"
#include "GUI.hpp"


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

	auto graphics = Graphics::init();

	if (!graphics.has_value()) {
		return SDL_APP_FAILURE;
	}

	new_state->graphics = std::move(graphics.value());

	init_imgui(new_state);

	if (!init_audio(new_state)) {
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* app_state) {
	AppState* state = static_cast<AppState*>(app_state);

	auto& fft_data = state->analysis_node->get_fft_data();
	state->graphics.texture.upload_texture(GL_RED, GL_FLOAT, fft_data.data());
	state->graphics.texture.generate_mipmap();

	int width = 0;
	int height = 0;

	SDL_GetWindowSize(state->main_window, &width, &height);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	state->graphics.vertex_array.bind();
	state->graphics.pipeline.bind();
	state->graphics.texture.bind(0);
	state->graphics.sampler.bind(0);

	state->graphics.fragment_shader.set_uniform("t", 0);
	state->graphics.fragment_shader.set_uniform("viewport_size", static_cast<float>(width), static_cast<float>(height));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	draw_gui(state);

	SDL_GL_SwapWindow(state->main_window);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* app_state, SDL_Event* event) {
	AppState* state = static_cast<AppState*>(app_state);

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
		
		case SDL_EVENT_KEY_DOWN: {
			if (event->key.key == SDLK_F10) {
				state->is_imgui_window_open = !state->is_imgui_window_open;
			}

			return SDL_APP_CONTINUE;
		}

		default:
			break;
	}

	return SDL_APP_CONTINUE;
}

// ReSharper disable once CppParameterNamesMismatch
void SDL_AppQuit(void* app_state, SDL_AppResult result) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	delete static_cast<AppState*>(app_state);
}
