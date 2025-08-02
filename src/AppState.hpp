#pragma once
#include <string>

#include <SDL3/SDL.h>

#include "miniaudio.h"
#include "AnalyserNode.hpp"
#include "Graphics.hpp"
#include "Window.hpp"

struct AppState {
	Window main_window;

	ma_engine* audio_engine;
	ma_sound* sound;
	std::unique_ptr<AnalyserNode> analysis_node;
	float audio_volume;
	bool is_playing;

	Graphics graphics;

	bool is_audio_file_selected;
	std::string audio_file_path;
	std::string audio_file_name;

	bool is_imgui_window_open;

	AppState();

	~AppState();
};
