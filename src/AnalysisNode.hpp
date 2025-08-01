#pragma once
#include <memory>
#include <vector>

#include "RollingBuffer.hpp"

#include <miniaudio.h>
#include <fftw3.h>

class AnalysisNode {
private:
	ma_node_base base{};
	RollingBuffer rolling_buffer;
	std::vector<float> fft_in_buffer;
	std::vector<fftwf_complex> fft_out_buffer;
	std::vector<float> mag_buffer;
	std::vector<float> hann_window;
	fftwf_plan fft_plan;

	void copy_audio_data();
	void apply_hann_window();

public:
	explicit AnalysisNode(size_t buffer_size);
	AnalysisNode(const AnalysisNode&) = delete;
	AnalysisNode(AnalysisNode&&) = delete;

	~AnalysisNode();

	const std::vector<float> &get_fft_data();

	static std::unique_ptr<AnalysisNode> create(ma_node_graph* node_graph, size_t buffer_size, ma_uint32 channel_count);
	static void process_node(ma_node* node, const float** frames_in, ma_uint32* frame_count_in, float** frames_out, ma_uint32* frame_count_out);
};

AnalysisNode* initialize_analysis_node(ma_node_graph* node_graph);
