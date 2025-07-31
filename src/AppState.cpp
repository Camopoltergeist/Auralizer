#include "AppState.hpp"

AppState::AppState() {
	main_window = nullptr;
	gl_context = nullptr;
	is_audio_file_selected = false;
	audio_file_path = "";
	audio_volume = 0.5f;
	audio_engine = nullptr;
	sound = nullptr;
	is_imgui_window_open = true;
	is_playing = false;
	audio_file_name = "";
	analysis_node = nullptr;
	buffer = std::array<float, ROLLING_BUFFER_SIZE>();
	buffer.fill(1.f);
}

AppState::~AppState() {
	if (sound != nullptr) {
		ma_sound_uninit(sound);
	}

	if (audio_engine != nullptr) {
		ma_engine_uninit(audio_engine);
	}
}
