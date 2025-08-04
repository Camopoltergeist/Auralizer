#pragma once
#include <memory>
#include <miniaudio.h>

#include "AudioContext.hpp"
#include "../RollingBuffer.hpp"

class CaptureDevice {
	ma_device device{};
	RollingBuffer buffer;

	static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);

public:
	CaptureDevice();
	CaptureDevice(const CaptureDevice&) = delete;
	CaptureDevice(CaptureDevice&&) = delete;
	~CaptureDevice();

	void copy_buffer(std::vector<float>& dest);
	void start();
	void stop();

	static std::unique_ptr<CaptureDevice> create(AudioContext& context, const ma_device_id& device_id);
};
