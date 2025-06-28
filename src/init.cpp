#include <glad.h>
#include <SDL3/SDL.h>

#include "AppState.hpp"

bool init_video_subsystem() {
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL Video subsystem: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool init_window(AppState* app_state) {
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

	SDL_Window* main_window = SDL_CreateWindow("Hello Window!", 1280, 720, window_flags);

	if (main_window == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
		return false;
	}

	app_state->main_window = main_window;

	return true;
}

bool init_opengl(AppState* app_state) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext gl_context = SDL_GL_CreateContext(app_state->main_window);

	if (gl_context == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	app_state->gl_context = gl_context;

	int version = gladLoadGL();

	if (version == 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OpenGL functions");
		return SDL_APP_FAILURE;
	}

	int opengl_major_version = 0;
	int opengl_minor_version = 0;
	int context_flags = 0;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &opengl_major_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &opengl_minor_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &context_flags);

	SDL_Log("OpenGL %i.%i", opengl_major_version, opengl_minor_version);

	if ((context_flags & SDL_GL_CONTEXT_DEBUG_FLAG) != 0) {
		SDL_Log("Debug Context");
	}
}