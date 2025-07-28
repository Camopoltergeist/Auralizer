#include "Sampler.h"

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

Sampler& Sampler::operator=(Sampler& other)
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
		return std::optional<Sampler>();
	}

	return std::make_optional<Sampler>(sampler);
}

void Sampler::set_min_filtering(GLenum filter_mode) const
{
	glSamplerParameteri(gl_name, GL_TEXTURE_MIN_FILTER, filter_mode);
}

void Sampler::set_mag_filtering(GLenum filter_mode) const
{
	glSamplerParameteri(gl_name, GL_TEXTURE_MAG_FILTER, filter_mode);
}

void Sampler::set_wrapping(GLenum wrap_mode) const
{
	glSamplerParameteri(gl_name, GL_TEXTURE_WRAP_S, wrap_mode);
	glSamplerParameteri(gl_name, GL_TEXTURE_WRAP_T, wrap_mode);
}

void Sampler::bind(GLuint texture_unit) const {
	glBindSampler(texture_unit, gl_name);
}