#pragma once
#include <optional>

#include <glad.h>

class VertexArray
{
private:
	GLuint gl_name;

	VertexArray(GLuint gl_name);

public:
	VertexArray(const VertexArray&) = delete;
	VertexArray(VertexArray&&) noexcept;
	~VertexArray();
	
	VertexArray& operator=(VertexArray& other);
	
	static std::optional<VertexArray> create();

	GLuint name() const;

	void enable_array_attrib(GLuint attrib_index) const;
	void set_attrib_format(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset) const;
};
