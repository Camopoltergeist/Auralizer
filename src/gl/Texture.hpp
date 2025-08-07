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

	Texture& operator=(Texture&&) noexcept;

	static std::optional<Texture> create(GLsizei width, GLsizei height, GLenum internal_format);

	void upload_texture(const GLenum format, const GLenum type, const void* data) const {
		glTextureSubImage2D(gl_name, 0, 0, 0, width, height, format, type, data);
	}

	void upload_texture_sub_image(const int x, const int y, const int width, const int height, const GLenum format, const GLenum type, const void* data) const
	{
		glTextureSubImage2D(gl_name, 0, x, y, width, height, format, type, data);
	}

	void generate_mipmap() const;

	void bind(GLuint texture_unit) const;

	[[nodiscard]] float get_width() const;
	[[nodiscard]] float get_height() const;
};

