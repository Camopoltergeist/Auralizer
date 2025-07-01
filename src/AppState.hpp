#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include "miniaudio.h"

#include <string>

struct AppState {
	SDL_Window* main_window;
	SDL_GLContext gl_context;

	ma_engine* audio_engine;
	ma_sound* sound;
	float audio_volume;

	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vertex_array_object;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint pipeline;

	bool is_audio_file_selected;
	std::string audio_file_path;

	AppState() {
		main_window = nullptr;
		gl_context = nullptr;
		vertex_buffer = 0;
		index_buffer = 0;
		vertex_array_object = 0;
		vertex_shader = 0;
		fragment_shader = 0;
		pipeline = 0;
		is_audio_file_selected = false;
		audio_file_path = "";
		audio_volume = 0.5f;
		audio_engine = nullptr;
		sound = nullptr;
	}

	~AppState() {
		if (sound != nullptr) {
			ma_sound_uninit(sound);
		}

		if (audio_engine != nullptr) {
			ma_engine_uninit(audio_engine);
		}

		if (vertex_array_object != 0) {
			glDeleteVertexArrays(1, &vertex_array_object);
		}

		if (vertex_buffer != 0) {
			glDeleteBuffers(1, &vertex_buffer);
		}

		if (index_buffer != 0) {
			glDeleteBuffers(1, &index_buffer);
		}

		if (pipeline != 0) {
			glDeleteProgramPipelines(1, &pipeline);
		}
		
		if (vertex_shader != 0) {
			glDeleteProgram(vertex_shader);
		}

		if (fragment_shader != 0) {
			glDeleteProgram(fragment_shader);
		}
	}
};