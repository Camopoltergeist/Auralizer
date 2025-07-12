#pragma once
#include "RollingBuffer.hpp"

#include <miniaudio.h>

struct AnalysisNode {
	ma_node_base base;
	RollingBuffer rolling_buffer;
};

AnalysisNode* initialize_analysis_node(ma_node_graph* node_graph);