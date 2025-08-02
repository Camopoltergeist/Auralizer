#include "AnalysisNode.hpp"
#undef min

#include <SDL3/SDL.h>

#include <cmath>
#include <numbers>

static ma_node_vtable process_node_vtable = {
	AnalysisNode::process_node,
	nullptr,
	1,
	1,
	MA_NODE_FLAG_PASSTHROUGH,
};

AnalysisNode::AnalysisNode(const size_t buffer_size) :
	rolling_buffer(buffer_size),
	fft_in_buffer(buffer_size, 0.0),
	fft_out_buffer(buffer_size / 2 + 1),
	mag_buffer(buffer_size / 2 + 1),
	hann_window(buffer_size, 0.0),
	fft_plan(nullptr)
{
	fft_plan = fftwf_plan_dft_r2c_1d(static_cast<int>(buffer_size), fft_in_buffer.data(), reinterpret_cast<fftwf_complex*>(fft_out_buffer.data()), FFTW_ESTIMATE);

	for (int i = 0; i < buffer_size; i++) {
		hann_window[i] = static_cast<float>(0.5 * (1 - std::cos(2 * std::numbers::pi * i / static_cast<double>(buffer_size - 1))));
	}
}

AnalysisNode::~AnalysisNode()
{
	ma_node_uninit(&base, nullptr);
}

const std::vector<float>& AnalysisNode::get_fft_data()
{
	copy_audio_data();
	apply_hann_window();

	fftwf_execute(fft_plan);

	const float max_mag = static_cast<float>(fft_in_buffer.size()) / 1.f;

	for (int i = 0; i < fft_out_buffer.size(); i++) {
		const float mag = std::sqrt(fft_out_buffer[i].a * fft_out_buffer[i].a + fft_out_buffer[i].b * fft_out_buffer[i].b);
		const float db = 20.f * std::log10(mag / max_mag + 0.000001f);

		const float max = -5.f;
		const float min = -100.f;

		mag_buffer[i] = std::clamp((db - min) / (max - min), 0.f, 1.f);
	}

	return mag_buffer;
}

void AnalysisNode::copy_audio_data()
{
	rolling_buffer.copy_buffer(fft_in_buffer.data());
}

void AnalysisNode::apply_hann_window()
{
	for (int i = 0; i < fft_in_buffer.size(); i++) {
		fft_in_buffer[i] *= hann_window[i];
	}
}

std::unique_ptr<AnalysisNode> AnalysisNode::create(ma_node_graph* node_graph, const size_t buffer_size, const ma_uint32 channel_count)
{
	const ma_uint32 input_channels[1] = {channel_count};
	const ma_uint32 output_channels[1] = {channel_count};

	ma_node_config node_config = ma_node_config_init();
	node_config.vtable = &process_node_vtable;
	node_config.pInputChannels = input_channels;
	node_config.pOutputChannels = output_channels;

	auto analysis_node = std::make_unique<AnalysisNode>(buffer_size);

	if (ma_node_init(node_graph, &node_config, nullptr, &analysis_node->base) != MA_SUCCESS) {
		SDL_Log("Failed to initialize analysis node");
		return nullptr;
	}

	return analysis_node;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void AnalysisNode::process_node(ma_node* node, const float** frames_in, ma_uint32* frame_count_in, float** frames_out, ma_uint32* frame_count_out)
{
	// NOLINT(*-non-const-parameter)
	const float* frames_in_0 = frames_in[0];

	auto* analysis_node = static_cast<AnalysisNode*>(node);

	analysis_node->rolling_buffer.write_interleaved(frames_in_0, *frame_count_in, false);
}
