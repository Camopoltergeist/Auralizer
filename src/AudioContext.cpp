#include "AudioContext.hpp"

#include <SDL3/SDL.h>

AudioContext::AudioContext() = default;

AudioContext::~AudioContext()
{
	ma_context_uninit(&context);
}

ma_context* AudioContext::get_context()
{
	return &context;
}

void AudioContext::update_device_list()
{
	ma_device_info* playback;
	ma_uint32 playback_count;
	ma_device_info* capture;
	ma_uint32 capture_count;

	if(ma_context_get_devices(&context, &playback , &playback_count, &capture, &capture_count) != MA_SUCCESS) {
		SDL_Log("Failed to enumerate devices");
		return;
	}

	playback_devices.clear();
	playback_devices.reserve(playback_count);

	for(int i = 0; i < playback_count; i++) {
		playback_devices.push_back(playback[i]);
	}

	capture_devices.clear();
	capture_devices.reserve(capture_count);

	for(int i = 0; i < capture_count; i++) {
		capture_devices.push_back(capture[i]);
	}

	for(auto& d : playback_devices) {
		SDL_Log("Device: %s", d.name);
	}
}

std::unique_ptr<AudioContext> AudioContext::create()
{
	auto context = std::make_unique<AudioContext>();

	if(ma_context_init(nullptr, 0, nullptr, &context->context) != MA_SUCCESS) {
		SDL_Log("Failed to create audio context");
		return nullptr;
	}

	return context;
}
