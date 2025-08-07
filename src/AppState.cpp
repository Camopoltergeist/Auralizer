#include "AppState.hpp"

AppState::AppState() : analyser(AUDIO_BUFFER_SIZE) {
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
	capture_device_selection = 0;
	last_frame = std::chrono::high_resolution_clock::now();
	last_frame_time = std::chrono::duration<float>::zero();
	fragment_shader_file_path = "./shaders/fragment.glsl";
	shader_reload_failure = false;
	audio_buffer_size = AUDIO_BUFFER_SIZE;
}

AppState::~AppState() {
	if (sound != nullptr) {
		ma_sound_uninit(sound);
	}
}

void AppState::reload_shader()
{
	shader_reload_failure = !graphics.load_shader(fragment_shader_file_path);
}
