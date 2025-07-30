#include "GLBuffer.h"

GLBuffer::GLBuffer(GLuint gl_name) : gl_name(gl_name) { }

GLBuffer::GLBuffer()
{
	gl_name = 0;
}

GLBuffer::GLBuffer(GLBuffer&& other) noexcept : gl_name(0)
{
	gl_name = other.gl_name;
	other.gl_name = 0;
}

GLBuffer::~GLBuffer()
{
	if (gl_name != 0) {
		glDeleteBuffers(1, &gl_name);
	}
}

GLBuffer& GLBuffer::operator=(GLBuffer&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteBuffers(1, &gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	return *this;
}

GLuint GLBuffer::name() const {
	return gl_name;
}
