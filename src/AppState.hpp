#pragma once
#include <string>

#include "miniaudio.h"
#include "audio/AnalyserNode.hpp"
#include "Graphics.hpp"
#include "Window.hpp"
#include "Analyser.hpp"
#include "audio/AudioContext.hpp"
#include "audio/AudioEngine.hpp"
#include "audio/CaptureDevice.hpp"

enum class AudioMode : int {
	AudioFile,
	Microphone
};

struct AppState {
	Window main_window;

	std::unique_ptr<AudioContext> audio_context;
	std::unique_ptr<AudioEngine> audio_engine;
	ma_sound* sound;
	std::unique_ptr<AnalyserNode> analysis_node;
	Analyser analyser;
	float audio_volume;
	bool is_playing;
	AudioMode audio_mode;
	std::unique_ptr<CaptureDevice> capture_device;

	Graphics graphics;

	bool is_audio_file_selected;
	std::string audio_file_path;
	std::string audio_file_name;

	bool is_imgui_window_open;

	AppState();

	~AppState();
};
