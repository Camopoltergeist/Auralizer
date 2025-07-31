#pragma once
#include <memory>

#include "RollingBuffer.hpp"

#include <miniaudio.h>

class AnalysisNode {
private:
	ma_node_base base{};
	RollingBuffer rolling_buffer;

public:
	AnalysisNode();
	AnalysisNode(const AnalysisNode&) = delete;
	AnalysisNode(AnalysisNode&&) = delete;

	~AnalysisNode();

	void copy_buffer(float* dest);

	static std::unique_ptr<AnalysisNode> create(ma_node_graph* node_graph, ma_uint32 channel_count);
	static void process_node(ma_node* node, const float** frames_in, ma_uint32* frame_count_in, float** frames_out, ma_uint32* frame_count_out);
};

AnalysisNode* initialize_analysis_node(ma_node_graph* node_graph);
