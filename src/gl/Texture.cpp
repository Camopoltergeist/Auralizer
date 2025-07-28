#include "Texture.h"

#include <SDL3/SDL.h>

#include <cmath>

Texture::Texture(GLuint gl_name, GLsizei width, GLsizei height) : gl_name(gl_name), width(width), height(height) { }

Texture::Texture() : gl_name(0), width(0), height(0) { }

Texture::Texture(Texture&& other) noexcept : gl_name(0), width(0), height(0)
{
	gl_name = other.gl_name;
	width = other.width;
	height = other.height;

	other.gl_name = 0;
	other.width = 0;
	other.height = 0;
}

Texture::~Texture()
{
	if (gl_name != 0) {
		glDeleteTextures(1, &gl_name);
	}
}

Texture& Texture::operator=(Texture& other)
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteTextures(1, &gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	return *this;
}

std::optional<Texture> Texture::create(GLsizei width, GLsizei height, GLenum internal_format) {
	GLuint texture = 0;

	glCreateTextures(GL_TEXTURE_2D, 1, &texture);

	if (texture == 0) {
		SDL_Log("Failed to create texture");
		return std::optional<Texture>();
	}

	int max_size = std::max(width, height);
	int levels = std::floor(std::log2(max_size)) + 1;

	glTextureStorage2D(texture, levels, internal_format, width, height);

	return std::make_optional<Texture>(texture, width, height);
}

void Texture::generate_mipmap() const
{
	glGenerateTextureMipmap(gl_name);
}

void Texture::bind(GLuint texture_unit) const
{
	glBindTextureUnit(texture_unit, gl_name);
}
