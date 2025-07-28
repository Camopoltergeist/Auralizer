#pragma once
#include <optional>
#include <vector>

#include <glad.h>

class Texture
{
private:
	GLuint gl_name;
	GLsizei width;
	GLsizei height;


public:
	Texture(GLuint gl_name, GLsizei width, GLsizei height);
	Texture();
	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept;
	~Texture();

	Texture& operator=(Texture&);

	static std::optional<Texture> create(GLsizei width, GLsizei height, GLenum internal_format);

	template <typename T>
	void upload_texture(GLenum format, GLenum type, const std::vector<T>& data) {
		glTextureSubImage2D(gl_name, 0, 0, 0, width, height, format, type, data.data());
	}

	void generate_mipmap() const;
};

