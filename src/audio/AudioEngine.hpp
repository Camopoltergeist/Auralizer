#pragma once
#include <memory>

#include "AudioContext.hpp"
#include "miniaudio.h"

class AudioEngine {
	ma_engine engine{};
	ma_resource_manager resource_manager{};

public:
	AudioEngine();
	AudioEngine(const AudioEngine&) = delete;
	AudioEngine(AudioEngine&&) = delete;
	~AudioEngine();

	ma_engine* get_engine();
	ma_node_graph* get_node_graph();

	void set_volume(float volume);

	static std::unique_ptr<AudioEngine> create(AudioContext& context);
};
