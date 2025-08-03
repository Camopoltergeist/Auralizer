#include "AppState.hpp"

AppState::AppState() : analyser(2048) {
	is_audio_file_selected = false;
	audio_file_path = "";
	audio_volume = 0.5f;
	audio_engine = nullptr;
	sound = nullptr;
	is_imgui_window_open = true;
	is_playing = false;
	audio_file_name = "";
	analysis_node = nullptr;
	audio_mode = AudioMode::AudioFile;
}

AppState::~AppState() {
	if (sound != nullptr) {
		ma_sound_uninit(sound);
	}
}
