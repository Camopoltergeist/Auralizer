#pragma once
#include <memory>
#include <vector>

#include "miniaudio.h"

class AudioContext {
	ma_context context{};
	std::vector<ma_device_info> capture_devices;
	std::vector<ma_device_info> playback_devices;

public:
	AudioContext();
	AudioContext(const AudioContext&) = delete;
	AudioContext(AudioContext&&) = delete;
	~AudioContext();

	ma_context* get_context();
	void update_device_list();

	static std::unique_ptr<AudioContext> create();
};
