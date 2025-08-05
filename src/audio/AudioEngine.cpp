#include "AudioEngine.hpp"
#include <vector>

#include <SDL3/SDL_log.h>
#include "extras/decoders/libvorbis/miniaudio_libvorbis.h"

static std::vector<ma_decoding_backend_vtable*> custom_backend_vtables = {
	ma_decoding_backend_libvorbis
};

AudioEngine::AudioEngine() = default;

AudioEngine::~AudioEngine()
{
	ma_resource_manager_uninit(&resource_manager);
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
	auto engine = std::make_unique<AudioEngine>();

	auto resource_manager_config = ma_resource_manager_config_init();
	resource_manager_config.ppCustomDecodingBackendVTables = custom_backend_vtables.data();
	resource_manager_config.customDecodingBackendCount = custom_backend_vtables.size();
	resource_manager_config.pCustomDecodingBackendUserData = nullptr;

	if(ma_resource_manager_init(&resource_manager_config, &engine->resource_manager) != MA_SUCCESS) {
		SDL_Log("Failed to initialize AudioEngine");
		return nullptr;
	}

	ma_engine_config config = ma_engine_config_init();
	config.pContext = context.get_context();
	config.pResourceManager = &engine->resource_manager;

	if(ma_engine_init(&config, &engine->engine) != MA_SUCCESS) {
		SDL_Log("Failed to initialize AudioEngine");
		return nullptr;
	}

	return engine;
}
