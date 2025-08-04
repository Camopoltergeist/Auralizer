#pragma once
#include <memory>
#include <vector>

#include "miniaudio.h"

class AudioContext {
	ma_context context{};
	std::vector<ma_device_info> playback_devices;
	std::vector<ma_device_info> capture_devices;
public:
	AudioContext();
	AudioContext(const AudioContext&) = delete;
	AudioContext(AudioContext&&) = delete;
	~AudioContext();

	std::vector<ma_device_info>& get_capture_devices();
	std::vector<const char*> get_capture_device_names();

	std::vector<ma_device_info>& get_playback_devices();
	std::vector<const char*> get_playback_device_names();

	ma_context* get_context();
	void update_device_list();

	static std::unique_ptr<AudioContext> create();
};
