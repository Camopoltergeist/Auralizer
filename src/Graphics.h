#pragma once
#include <glad.h>
#include <optional>

#include "gl/VertexArray.h"
#include "gl/GLBuffer.h"
#include "gl/Shader.h"
#include "gl/Pipeline.h"
#include "gl/Texture.h"
#include "gl/Sampler.h"

class Graphics
{
public:
	VertexArray vertex_array;
	GLBuffer vertex_buffer;
	GLBuffer index_buffer;
	Shader vertex_shader;
	Shader fragment_shader;
	Pipeline pipeline;
	Texture texture;
	Sampler sampler;

public:
	Graphics();
	Graphics(
		VertexArray vertex_array,
		GLBuffer vertex_buffer,
		GLBuffer index_buffer,
		Shader vertex_shader,
		Shader fragment_shader,
		Pipeline pipeline,
		Texture texture,
		Sampler sampler
	);

	static std::optional<Graphics> init();

	~Graphics();
};

