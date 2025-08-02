#pragma once
#include <glad.h>
#include <optional>

#include "gl/VertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/Shader.hpp"
#include "gl/Pipeline.hpp"
#include "gl/Texture.hpp"
#include "gl/Sampler.hpp"

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

	Graphics& operator=(Graphics&& other) noexcept;

	~Graphics();
};

