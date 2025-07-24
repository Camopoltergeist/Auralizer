#pragma once
#include <glad.h>
#include <optional>

#include "gl/VertexArray.h"

class Graphics
{
private:
	VertexArray vertex_array;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint pipeline;


public:
	Graphics(VertexArray vertex_array);

	static std::optional<Graphics> init();

	~Graphics();
};

