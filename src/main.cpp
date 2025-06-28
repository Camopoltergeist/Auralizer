#include <iostream>

#include <glad.h>
#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

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

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* app_state) {
	AppState* state = static_cast<AppState*>(app_state);

	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(state->main_window);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* app_state, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* app_state, SDL_AppResult result) {
	delete app_state;
}