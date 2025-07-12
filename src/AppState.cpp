#include "AppState.hpp"

AppState::AppState() {
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
	is_imgui_window_open = true;
	is_playing = false;
	audio_file_name = "";
	analysis_node = nullptr;
	buffer = std::array<float, ROLLING_BUFFER_SIZE>();
	buffer.fill(0);
}

AppState::~AppState() {
	if (analysis_node != nullptr) {
		ma_node_uninit(&analysis_node->base, nullptr);
	}

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
