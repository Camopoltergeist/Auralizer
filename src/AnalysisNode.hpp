#pragma once
#include <miniaudio.h>

struct AnalysisNode {
	ma_node_base base;
};

AnalysisNode* initialize_analysis_node(ma_node_graph* node_graph);