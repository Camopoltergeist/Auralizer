#include "VertexArray.h"

#include <SDL3/SDL.h>

VertexArray::VertexArray(GLuint gl_name) : gl_name(gl_name) { }

VertexArray::VertexArray(VertexArray&& other) noexcept : gl_name(0)
{
	gl_name = other.gl_name;
	other.gl_name = 0;
}

VertexArray& VertexArray::operator=(VertexArray& other)
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteVertexArrays(1, &gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	return *this;
}

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

	return std::make_optional<VertexArray>(vertex_array_object);
}

VertexArray::~VertexArray()
{
	if (gl_name != 0) {
		glDeleteVertexArrays(1, &gl_name);
	}
}

GLuint VertexArray::name() const
{
	return gl_name;
}

void VertexArray::enable_array_attrib(GLuint attrib_index) const
{
	glEnableVertexArrayAttrib(gl_name, attrib_index);
}

void VertexArray::set_attrib_format(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset) const
{
	glVertexArrayAttribFormat(gl_name, attrib_index, size, type, normalized, relative_offset);
}
