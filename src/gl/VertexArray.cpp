#include "VertexArray.h"

#include <SDL3/SDL.h>

VertexArray::VertexArray(GLuint gl_name) : gl_name(gl_name) { }

std::optional<VertexArray> VertexArray::create()
{
	GLuint vertex_array_object = 0;

	glCreateVertexArrays(1, &vertex_array_object);

	if (vertex_array_object == 0) {
		SDL_Log("Failed to create vertex array object");
		return std::optional<VertexArray>();
	}

	glEnableVertexArrayAttrib(vertex_array_object, 0);
	glEnableVertexArrayAttrib(vertex_array_object, 1);

	glVertexArrayAttribFormat(vertex_array_object, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vertex_array_object, 1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2);

	VertexArray vertex_array(vertex_array_object);

	return std::optional<VertexArray>(vertex_array);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &gl_name);
}
