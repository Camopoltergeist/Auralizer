#include "AnalysisNode.hpp"
#undef min

#include <SDL3/SDL.h>

#include <algorithm>

static ma_node_vtable process_node_vtable = {
	AnalysisNode::process_node,
	nullptr,
	1,
	1,
	MA_NODE_FLAG_PASSTHROUGH,
};

AnalysisNode::AnalysisNode() = default;

AnalysisNode::~AnalysisNode() {
	ma_node_uninit(&base, nullptr);
}

void AnalysisNode::copy_buffer(float *dest) {
	rolling_buffer.copy_buffer(dest);
}

std::unique_ptr<AnalysisNode> AnalysisNode::create(ma_node_graph *node_graph, const ma_uint32 channel_count) {
	const ma_uint32 input_channels[1] = { channel_count };
	const ma_uint32 output_channels[1] = { channel_count };

	ma_node_config node_config = ma_node_config_init();
	node_config.vtable = &process_node_vtable;
	node_config.pInputChannels = input_channels;
	node_config.pOutputChannels = output_channels;

	auto analysis_node = std::make_unique<AnalysisNode>();

	if (ma_node_init(node_graph, &node_config, nullptr, &analysis_node->base) != MA_SUCCESS) {
		SDL_Log("Failed to initialize analysis node");
		return nullptr;
	}

	return analysis_node;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void AnalysisNode::process_node(ma_node *node, const float **frames_in, ma_uint32 *frame_count_in, float **frames_out, ma_uint32 *frame_count_out) { // NOLINT(*-non-const-parameter)
	const float* frames_in_0 = frames_in[0];

	auto* analysis_node = static_cast<AnalysisNode*>(node);

	analysis_node->rolling_buffer.write_interleaved(frames_in_0, *frame_count_in, false);
}
