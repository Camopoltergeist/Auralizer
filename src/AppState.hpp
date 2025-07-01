#pragma once

#include <glad.h>
#include <SDL3/SDL.h>

#include <string>

struct AppState {
	SDL_Window* main_window;
	SDL_GLContext gl_context;

	SDL_AudioDeviceID audio_device;
	SDL_AudioSpec audio_device_spec;
	Uint8* audio_data;
	Uint32 audio_data_length;
	SDL_AudioStream* audio_stream;

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
		audio_device = 0;
		audio_device_spec = {};
		SDL_zero(audio_device_spec);
		audio_data = nullptr;
		audio_data_length = 0;
		audio_stream = nullptr;
		vertex_buffer = 0;
		index_buffer = 0;
		vertex_array_object = 0;
		vertex_shader = 0;
		fragment_shader = 0;
		pipeline = 0;
		is_audio_file_selected = false;
		audio_file_path = "";
	}

	~AppState() {
		SDL_DestroyAudioStream(audio_stream);
		SDL_CloseAudioDevice(audio_device);
		SDL_free(audio_data);

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