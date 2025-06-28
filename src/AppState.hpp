#pragma once

#include <glad.h>
#include <SDL3/SDL.h>

struct AppState {
	SDL_Window* main_window;
	SDL_GLContext gl_context;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vertex_array_object;

	AppState() {
		main_window = nullptr;
		gl_context = nullptr;
		vertex_buffer = 0;
		index_buffer = 0;
		vertex_array_object = 0;
	}

	~AppState() {
		if (vertex_array_object != 0) {
			glDeleteVertexArrays(1, &vertex_array_object);
		}

		if (vertex_buffer != 0) {
			glDeleteBuffers(1, &vertex_buffer);
		}

		if (index_buffer != 0) {
			glDeleteBuffers(1, &index_buffer);
		}

	}
};