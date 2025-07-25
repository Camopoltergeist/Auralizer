#pragma once
#include <glad.h>
#include <optional>

#include "gl/VertexArray.h"
#include "gl/GLBuffer.h"
#include "gl/Shader.h"

class Graphics
{
private:
	VertexArray vertex_array;
	GLBuffer vertex_buffer;
	GLBuffer index_buffer;
	Shader vertex_shader;
	Shader fragment_shader;
	GLuint pipeline;

public:
	Graphics(VertexArray vertex_array, GLBuffer vertex_buffer, GLBuffer index_buffer, Shader vertex_shader, Shader fragment_shader);

	static std::optional<Graphics> init();

	~Graphics();
};

