#pragma once
#include <vector>

#include <fftw3.h>

// fftwf_complex is defined as the following:
//
// typedef float fftwf_complex[2]
//
// which worked fine with MinGW, but not with MSVC.
// Apparently you're not supposed to store fixed size arrays in vectors, so this struct circumvents that.
struct fuckyou {
	float a = 0.f;
	float b = 0.f;
};

class Analyser {
	std::vector<float> fft_in_buffer;
	std::vector<fuckyou> fft_out_buffer;
	std::vector<float> mag_buffer;
	std::vector<float> hann_window;
	fftwf_plan fft_plan = nullptr;

	void initialize_hann_window();
	void apply_hann_window();

public:
	float min_db = -100.f;
	float max_db = -5.f;

	explicit Analyser(size_t buffer_size);
	Analyser(const Analyser& other);
	Analyser(Analyser&&) = delete;
	~Analyser();

	Analyser& operator=(const Analyser& other);

	std::vector<float>& in_buffer();
	const std::vector<float>& get_fft_data();
};
