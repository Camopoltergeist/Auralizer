#pragma once

#include <array>
#include <algorithm>
#include <mutex>

#define ROLLING_BUFFER_SIZE 1024

class RollingBuffer
{
	std::array<float, ROLLING_BUFFER_SIZE> buffer;
	int end;
	std::mutex mutex;

public:
	RollingBuffer() : buffer(), end(0) {
		buffer.fill(0.0);
	}

	void write_interleaved(const float* data, int frame_count, bool odd) {
		mutex.lock();

		int space_left = static_cast<int>(buffer.size()) - end;

		int write_start = std::max(0, frame_count - (int)buffer.size());

		frame_count = std::min(frame_count, (int)buffer.size());

		int write_1_size = std::min(space_left, frame_count);
		int write_2_size = frame_count - space_left;

		for (int i = 0; i < write_1_size; i++) {
			int offset = i * 2 + (int)odd;

			buffer[end + i] = data[offset];
		}

		for (int i = 0; i < write_2_size; i++) {
			int offset = (i + write_1_size) * 2 + (int)odd;

			buffer[i] = data[offset];
		}

		end = static_cast<int>((buffer.size() + write_2_size) % buffer.size());

		mutex.unlock();
	}

	size_t get_size() {
		return buffer.size();
	}

	void copy_buffer(float* dest) {
		mutex.lock();

		int space_left = static_cast<int>(buffer.size()) - end;

		int copy_1_size = space_left;
		int copy_2_size = static_cast<int>(buffer.size()) - space_left;

		for (int i = 0; i < copy_1_size; i++) {
			int offset = end + i;

			dest[i] = buffer[offset];
		}

		for (int i = 0; i < copy_2_size; i++) {
			dest[i] = buffer[i];
		}

		mutex.unlock();
	}
};

