#pragma once

#include <SDL3/SDL.h>

struct AppState {
	SDL_Window* main_window;
	SDL_GLContext gl_context;
};