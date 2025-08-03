#pragma once
#include <memory>

#include "miniaudio.h"

class AudioContext {
	ma_context context{};

public:
	AudioContext();
	AudioContext(const AudioContext&) = delete;
	AudioContext(AudioContext&&) = delete;
	~AudioContext();

	static std::unique_ptr<AudioContext> create();
};
