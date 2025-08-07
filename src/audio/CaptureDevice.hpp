#pragma once
#include <memory>
#include <miniaudio.h>

#include "AudioContext.hpp"
#include "../RollingBuffer.hpp"

class CaptureDevice {
	ma_device device{};
	RollingBuffer left_channel_buffer;
	RollingBuffer right_channel_buffer;

	static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);

public:
	explicit CaptureDevice(size_t buffer_size);
	CaptureDevice(const CaptureDevice&) = delete;
	CaptureDevice(CaptureDevice&&) = delete;
	~CaptureDevice();

	void copy_left_channel_buffer(std::vector<float>& dest);
	void copy_right_channel_buffer(std::vector<float>& dest);
	void start();
	void stop();

	static std::unique_ptr<CaptureDevice> create(size_t buffer_size, AudioContext& context, const ma_device_id& device_id);
};
