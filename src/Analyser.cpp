#include "Analyser.hpp"
#include <cmath>
#include <algorithm>
#include <numbers>

void Analyser::initialize_hann_window()
{
	for (int i = 0; i < hann_window.size(); i++) {
		hann_window[i] = static_cast<float>(0.5 * (1 - std::cos(2 * std::numbers::pi * i / static_cast<double>(hann_window.size() - 1))));
	}
}

void Analyser::apply_hann_window()
{
	for (int i = 0; i < fft_in_buffer.size(); i++) {
		fft_in_buffer[i] *= hann_window[i];
	}
}

Analyser::Analyser(const size_t buffer_size) :
	fft_in_buffer(buffer_size, 0.0),
	fft_out_buffer(buffer_size / 2 + 1),
	mag_buffer(buffer_size / 2 + 1),
	hann_window(buffer_size, 0.0)
{
	fft_plan = fftwf_plan_dft_r2c_1d(static_cast<int>(buffer_size), fft_in_buffer.data(), reinterpret_cast<fftwf_complex*>(fft_out_buffer.data()), FFTW_ESTIMATE);
	initialize_hann_window();
}

Analyser::Analyser(const Analyser& other) :
	fft_in_buffer(other.fft_in_buffer),
	fft_out_buffer(other.fft_out_buffer),
	mag_buffer(other.mag_buffer),
	hann_window(other.hann_window)
{
	fft_plan = fftwf_plan_dft_r2c_1d(static_cast<int>(fft_in_buffer.size()), fft_in_buffer.data(), reinterpret_cast<fftwf_complex*>(fft_out_buffer.data()), FFTW_ESTIMATE);
}

Analyser::~Analyser()
{
	fftwf_destroy_plan(fft_plan);
}

Analyser& Analyser::operator=(const Analyser& other)
{
	this->~Analyser();

	fft_in_buffer = other.fft_in_buffer;
	fft_out_buffer = other.fft_out_buffer;
	mag_buffer = other.mag_buffer;
	hann_window = other.hann_window;
	fft_plan = fftwf_plan_dft_r2c_1d(static_cast<int>(fft_in_buffer.size()), fft_in_buffer.data(), reinterpret_cast<fftwf_complex*>(fft_out_buffer.data()), FFTW_ESTIMATE);

	return *this;
}

std::vector<float>& Analyser::in_buffer()
{
	return fft_in_buffer;
}

const std::vector<float>& Analyser::get_fft_data()
{
	apply_hann_window();

	fftwf_execute(fft_plan);

	const float max_mag = static_cast<float>(fft_in_buffer.size()) / 1.f;

	for (int i = 0; i < fft_out_buffer.size(); i++) {
		const float mag = std::sqrt(fft_out_buffer[i].a * fft_out_buffer[i].a + fft_out_buffer[i].b * fft_out_buffer[i].b);
		const float db = 20.f * std::log10(mag / max_mag + 0.000001f);

		mag_buffer[i] = std::clamp((db - min_db) / (max_db - min_db), -1.f, 1.f);
	}

	return mag_buffer;
}
