#pragma once
#include <optional>

#include <glad.h>

#include "GLBuffer.h"

class VertexArray
{
private:
	GLuint gl_name;

	VertexArray(GLuint gl_name);

public:
	VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray(VertexArray&&) noexcept;
	~VertexArray();
	
	VertexArray& operator=(VertexArray& other);
	
	static std::optional<VertexArray> create();

	GLuint name() const;

	void enable_array_attrib(GLuint attrib_index) const;
	void set_attrib_format(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset) const;
	void bind_buffer_to_binding_index(GLuint binding_index, const GLBuffer& buffer, GLintptr offset, GLsizei stride) const;
	void bind_attrib_to_binding_index(GLuint binding_index, GLuint attrib_index) const;
	void bind_element_buffer(const GLBuffer& element_buffer) const;

	void bind() const;
};
