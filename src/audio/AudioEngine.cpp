#include "AudioEngine.hpp"

#include <SDL3/SDL_log.h>

AudioEngine::AudioEngine() = default;

AudioEngine::~AudioEngine()
{
	ma_engine_uninit(&engine);
}

ma_engine* AudioEngine::get_engine()
{
	return &engine;
}

ma_node_graph* AudioEngine::get_node_graph()
{
	return ma_engine_get_node_graph(&engine);
}

void AudioEngine::set_volume(const float volume)
{
	ma_engine_set_volume(&engine, volume);
}

std::unique_ptr<AudioEngine> AudioEngine::create(AudioContext& context)
{
	ma_engine_config config = ma_engine_config_init();
	config.pContext = context.get_context();

	auto engine = std::make_unique<AudioEngine>();

	if(ma_engine_init(&config, &engine->engine) != MA_SUCCESS) {
		SDL_Log("Failed to initialize AudioEngine");
		return nullptr;
	}

	return engine;
}
