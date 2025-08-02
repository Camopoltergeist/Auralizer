#include "Sampler.hpp"

#include <SDL3/SDL.h>

Sampler::Sampler(GLuint gl_name) : gl_name(gl_name) { }

Sampler::Sampler() : gl_name(0) { }

Sampler::Sampler(Sampler&& other) noexcept
{
	gl_name = other.gl_name;
	other.gl_name = 0;
}

Sampler::~Sampler()
{
	if (gl_name != 0) {
		glDeleteSamplers(1, &gl_name);
	}
}

Sampler& Sampler::operator=(Sampler&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteSamplers(1, &gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	return *this;
}

std::optional<Sampler> Sampler::create() {
	GLuint sampler = 0;

	glCreateSamplers(1, &sampler);

	if (sampler == 0) {
		SDL_Log("Failed to create sampler");
		return std::nullopt;
	}

	return std::make_optional<Sampler>(Sampler(sampler));
}

void Sampler::set_min_filtering(const GLint filter_mode) const
{
	glSamplerParameteri(gl_name, GL_TEXTURE_MIN_FILTER, filter_mode);
}

void Sampler::set_mag_filtering(const GLint filter_mode) const
{
	glSamplerParameteri(gl_name, GL_TEXTURE_MAG_FILTER, filter_mode);
}

void Sampler::set_wrapping(const GLint wrap_mode) const
{
	glSamplerParameteri(gl_name, GL_TEXTURE_WRAP_S, wrap_mode);
	glSamplerParameteri(gl_name, GL_TEXTURE_WRAP_T, wrap_mode);
}

void Sampler::bind(const GLuint texture_unit) const {
	glBindSampler(texture_unit, gl_name);
}
