#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include "miniaudio.h"
#include "AnalysisNode.hpp"

#include <string>

struct AppState {
	SDL_Window* main_window;
	SDL_GLContext gl_context;

	ma_engine* audio_engine;
	ma_sound* sound;
	AnalysisNode* analysis_node;
	float audio_volume;
	bool is_playing;

	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vertex_array_object;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint pipeline;

	bool is_audio_file_selected;
	std::string audio_file_path;
	std::string audio_file_name;

	bool is_imgui_window_open;

	AppState();
	~AppState();
};