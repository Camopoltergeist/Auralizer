#pragma once
#include <glad.h>
#include <optional>

class Graphics
{
private:
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vertex_array_object;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint pipeline;

	Graphics();

public:

	static std::optional<Graphics> init();

	~Graphics();
};

