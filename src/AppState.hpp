#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include "miniaudio.h"
#include "AnalysisNode.hpp"
#include "Graphics.h"

#include <string>
#include <array>

struct AppState {
	SDL_Window* main_window;
	SDL_GLContext gl_context;

	ma_engine* audio_engine;
	ma_sound* sound;
	AnalysisNode* analysis_node;
	float audio_volume;
	bool is_playing;

	std::array<float, ROLLING_BUFFER_SIZE> buffer{};

	Graphics graphics;

	bool is_audio_file_selected;
	std::string audio_file_path;
	std::string audio_file_name;

	bool is_imgui_window_open;

	AppState();
	~AppState();
};
