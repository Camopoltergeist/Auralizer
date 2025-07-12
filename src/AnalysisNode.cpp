#include "AnalysisNode.hpp"
#undef min

#include <SDL3/SDL.h>

#include <algorithm>
#include <cstring>

static void process_node(ma_node* node, const float** frames_in, ma_uint32* frame_count_in, float** frames_out, ma_uint32* frame_count_out) {
	const float* frames_in_0 = frames_in[0];
	float* frames_out_0 = frames_out[0];

	ma_uint32 channel_count = ma_node_get_input_channels(node, 0);
	unsigned int element_count = *frame_count_out * channel_count;

	AnalysisNode* analysis_node = static_cast<AnalysisNode*>(node);

	analysis_node->rolling_buffer.write_interleaved(frames_in_0, *frame_count_in, false);
}

static ma_node_vtable process_node_vtable = {
	process_node,
	nullptr,
	1,
	1,
	MA_NODE_FLAG_PASSTHROUGH,
};

AnalysisNode* initialize_analysis_node(ma_node_graph* node_graph) {
	ma_uint32 input_channels[1] = { 2 };
	ma_uint32 output_channels[1] = { 2 };

	ma_node_config node_config = ma_node_config_init();
	node_config.vtable = &process_node_vtable;
	node_config.pInputChannels = input_channels;
	node_config.pOutputChannels = output_channels;

	AnalysisNode* analysis_node = new AnalysisNode;
	
	if (ma_node_init(node_graph, &node_config, nullptr, &analysis_node->base) != MA_SUCCESS) {
		SDL_Log("Failed to initialize analysis node");
		return nullptr;
	}

	return analysis_node;
}