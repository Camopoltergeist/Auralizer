#pragma once

#include <vector>
#include <algorithm>
#include <mutex>

class RollingBuffer {
	std::vector<float> buffer;
	int end;
	std::mutex mutex;

public:
	explicit RollingBuffer(const size_t buffer_size) :
		buffer(buffer_size, 0.0),
		end(0) { }

	void write_interleaved(const float* data, int frame_count, const bool odd) {
		mutex.lock();

		const int space_left = static_cast<int>(buffer.size()) - end;

		frame_count = std::min(frame_count, static_cast<int>(buffer.size()));

		const int write_1_size = std::min(space_left, frame_count);
		const int write_2_size = frame_count - space_left;

		for (int i = 0; i < write_1_size; i++) {
			const int offset = i * 2 + static_cast<int>(odd);

			buffer[end + i] = data[offset];
		}

		for (int i = 0; i < write_2_size; i++) {
			const int offset = (i + write_1_size) * 2 + static_cast<int>(odd);

			buffer[i] = data[offset];
		}

		end = static_cast<int>((buffer.size() + write_2_size) % buffer.size());

		mutex.unlock();
	}

	[[nodiscard]] size_t size() const {
		return buffer.size();
	}

	void copy_buffer(float* dest) {
		mutex.lock();

		const int space_left = static_cast<int>(buffer.size()) - end;

		const int copy_1_size = space_left;
		const int copy_2_size = static_cast<int>(buffer.size()) - space_left;

		for (int i = 0; i < copy_1_size; i++) {
			const int offset = end + i;

			dest[i] = buffer[offset];
		}

		for (int i = 0; i < copy_2_size; i++) {
			dest[i + copy_1_size] = buffer[i];
		}

		mutex.unlock();
	}
};
