#pragma once
#include <memory>
#include <vector>

#include "RollingBuffer.hpp"

#include <miniaudio.h>

#include "Analyser.hpp"

class AnalyserNode {
private:
	ma_node_base base{};
	RollingBuffer rolling_buffer;

public:
	explicit AnalyserNode(size_t buffer_size);
	AnalyserNode(const AnalyserNode&) = delete;
	AnalyserNode(AnalyserNode&&) = delete;
	~AnalyserNode();

	void copy_buffer(std::vector<float>& dest);

	static std::unique_ptr<AnalyserNode> create(ma_node_graph* node_graph, size_t buffer_size, ma_uint32 channel_count);

	static void process_node(ma_node* node, const float** frames_in, ma_uint32* frame_count_in, float** frames_out, ma_uint32* frame_count_out);
};

AnalyserNode* initialize_analysis_node(ma_node_graph* node_graph);
