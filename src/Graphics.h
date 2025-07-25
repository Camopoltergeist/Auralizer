#pragma once
#include <glad.h>
#include <optional>

#include "gl/VertexArray.h"
#include "gl/GLBuffer.h"

class Graphics
{
private:
	VertexArray vertex_array;
	GLBuffer vertex_buffer;
	GLBuffer index_buffer;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint pipeline;


public:
	Graphics(VertexArray vertex_array, GLBuffer vertex_buffer, GLBuffer index_buffer);

	static std::optional<Graphics> init();

	~Graphics();
};

