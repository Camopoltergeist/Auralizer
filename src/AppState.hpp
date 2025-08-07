#pragma once
#include <string>
#include <chrono>

#include "miniaudio.h"
#include "audio/AnalyserNode.hpp"
#include "Graphics.hpp"
#include "Window.hpp"
#include "Analyser.hpp"
#include "audio/AudioContext.hpp"
#include "audio/AudioEngine.hpp"
#include "audio/CaptureDevice.hpp"

constexpr size_t AUDIO_BUFFER_SIZE = 2048;

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
	int capture_device_selection;

	std::chrono::time_point<std::chrono::high_resolution_clock> last_frame;
	std::chrono::duration<float> last_frame_time{};

	Graphics graphics;
	std::string fragment_shader_file_path;
	bool shader_reload_failure;

	bool is_audio_file_selected;
	std::string audio_file_path;
	std::string audio_file_name;

	bool is_imgui_window_open;

	size_t audio_buffer_size;

	AppState();

	~AppState();

	void reload_shader();
};
