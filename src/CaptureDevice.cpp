#include "CaptureDevice.hpp"

#include <SDL3/SDL.h>

void CaptureDevice::data_callback(ma_device* device, void* output, const void* input, const ma_uint32 frame_count)
{
	auto* capture_device = reinterpret_cast<CaptureDevice*>(device);

	capture_device->buffer.write_interleaved(static_cast<const float*>(input), static_cast<int>(frame_count), false);
}

CaptureDevice::CaptureDevice() : buffer(2048)
{ }

CaptureDevice::~CaptureDevice()
{
	ma_device_uninit(&device);
}

std::unique_ptr<CaptureDevice> CaptureDevice::create()
{
	ma_device_config capture_config = ma_device_config_init(ma_device_type_capture);
	capture_config.capture.format = ma_format_f32;
	capture_config.capture.channels = 2;
	capture_config.sampleRate = 48000;
	capture_config.dataCallback = data_callback;

	auto capture_device = std::make_unique<CaptureDevice>();

	if(ma_device_init(nullptr, &capture_config, &capture_device->device) != MA_SUCCESS) {
		SDL_Log("Failed to create capture device");
		return nullptr;
	}

	ma_device_start(&capture_device->device);

	return capture_device;
}
