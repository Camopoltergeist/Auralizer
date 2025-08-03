#include "AudioContext.hpp"

#include <SDL3/SDL.h>

AudioContext::AudioContext() = default;

AudioContext::~AudioContext()
{
	ma_context_uninit(&context);
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
