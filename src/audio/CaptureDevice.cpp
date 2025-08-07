#include "CaptureDevice.hpp"

#include <SDL3/SDL.h>

#include "AudioContext.hpp"

void CaptureDevice::data_callback(ma_device* device, void* output, const void* input, const ma_uint32 frame_count)
{
	auto* capture_device = reinterpret_cast<CaptureDevice*>(device);

	capture_device->left_channel_buffer.write_interleaved(static_cast<const float*>(input), static_cast<int>(frame_count), false);
	capture_device->right_channel_buffer.write_interleaved(static_cast<const float*>(input), static_cast<int>(frame_count), true);
}

CaptureDevice::CaptureDevice(const size_t buffer_size) : left_channel_buffer(buffer_size), right_channel_buffer(buffer_size)
{ }

CaptureDevice::~CaptureDevice()
{
	ma_device_uninit(&device);
}

void CaptureDevice::copy_left_channel_buffer(std::vector<float>& dest)
{
	left_channel_buffer.copy_buffer(dest.data());
}

void CaptureDevice::copy_right_channel_buffer(std::vector<float>& dest)
{
	right_channel_buffer.copy_buffer(dest.data());
}

void CaptureDevice::start()
{
	ma_device_start(&device);
}

void CaptureDevice::stop()
{
	ma_device_stop(&device);
}

std::unique_ptr<CaptureDevice> CaptureDevice::create(const size_t buffer_size, AudioContext& context, const ma_device_id& device_id)
{
	ma_device_config capture_config = ma_device_config_init(ma_device_type_capture);
	capture_config.capture.format = ma_format_f32;
	capture_config.capture.channels = 2;
	capture_config.capture.pDeviceID = &device_id;
	capture_config.sampleRate = 48000;
	capture_config.dataCallback = data_callback;

	auto capture_device = std::make_unique<CaptureDevice>(buffer_size);

	if(ma_device_init(context.get_context(), &capture_config, &capture_device->device) != MA_SUCCESS) {
		SDL_Log("Failed to create capture device");
		return nullptr;
	}

	ma_device_start(&capture_device->device);

	return capture_device;
}
