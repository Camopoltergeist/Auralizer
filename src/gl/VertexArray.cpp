#include "VertexArray.hpp"

#include <SDL3/SDL.h>

VertexArray::VertexArray(GLuint gl_name) : gl_name(gl_name) { }

VertexArray::VertexArray()
{
	gl_name = 0;
}

VertexArray::VertexArray(VertexArray&& other) noexcept : gl_name(0)
{
	gl_name = other.gl_name;
	other.gl_name = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
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
		return std::nullopt;
	}

	return std::make_optional<VertexArray>(VertexArray(vertex_array_object));
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

void VertexArray::set_attrib_format(const GLuint attrib_index, const GLint size, GLenum type, const GLboolean normalized, const GLuint relative_offset) const
{
	glVertexArrayAttribFormat(gl_name, attrib_index, size, type, normalized, relative_offset);
}

void VertexArray::bind_buffer_to_binding_index(const GLuint binding_index, const GLBuffer& buffer, const GLintptr offset, const GLsizei stride) const
{
	glVertexArrayVertexBuffer(gl_name, binding_index, buffer.name(), offset, stride);
}

void VertexArray::bind_attrib_to_binding_index(const GLuint binding_index, const GLuint attrib_index) const
{
	glVertexArrayAttribBinding(gl_name, attrib_index, binding_index);
}

void VertexArray::bind_element_buffer(const GLBuffer& element_buffer) const
{
	glVertexArrayElementBuffer(gl_name, element_buffer.name());
}

void VertexArray::bind() const
{
	glBindVertexArray(gl_name);
}
