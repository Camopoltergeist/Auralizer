#pragma once
#include <optional>
#include <vector>

#include <glad.h>
#include <SDL3/SDL.h>

class GLBuffer
{
private:
	GLuint gl_name;

	GLBuffer(GLuint gl_name);

public:
	GLBuffer(const GLBuffer&) = delete;
	GLBuffer(GLBuffer&&) noexcept;
	~GLBuffer();

	GLBuffer& operator=(GLBuffer&);

	template <typename T>
	static std::optional<GLBuffer> create(const std::vector<T>& buffer_data) {
		GLuint vertex_buffer = 0;

		glCreateBuffers(1, &vertex_buffer);

		if (vertex_buffer == 0) {
			SDL_Log("Failed to create vertex buffer");
			return std::optional<GLBuffer>();
		}

		size_t byte_size = buffer_data.size() * sizeof(T);

		glNamedBufferStorage(vertex_buffer, byte_size, buffer_data.data(), 0);

		return std::make_optional<GLBuffer>(vertex_buffer);
	}

	GLuint name() const;
};

