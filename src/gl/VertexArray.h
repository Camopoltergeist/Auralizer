#pragma once
#include <optional>

#include <glad.h>

class VertexArray
{
private:
	GLuint gl_name;

	VertexArray(GLuint gl_name);

public:
	static std::optional<VertexArray> create();

	~VertexArray();
};

