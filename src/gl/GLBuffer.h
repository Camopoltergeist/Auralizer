#pragma once
#include <optional>
#include <vector>

#include <glad.h>
#include <SDL3/SDL.h>

class GLBuffer
{
private:
	GLuint gl_name;

	explicit GLBuffer(GLuint gl_name);

public:
	GLBuffer();
	GLBuffer(const GLBuffer&) = delete;
	GLBuffer(GLBuffer&&) noexcept;
	~GLBuffer();

	GLBuffer& operator=(GLBuffer&&) noexcept;

	template <typename T>
	static std::optional<GLBuffer> create(const std::vector<T>& buffer_data) {
		GLuint buffer = 0;

		glCreateBuffers(1, &buffer);

		if (buffer == 0) {
			SDL_Log("Failed to create vertex buffer");
			return std::nullopt;
		}

		const size_t byte_size = buffer_data.size() * sizeof(T);

		glNamedBufferStorage(buffer, byte_size, buffer_data.data(), 0);

		return std::make_optional<GLBuffer>(GLBuffer(buffer));
	}

	[[nodiscard]] GLuint name() const;
};

