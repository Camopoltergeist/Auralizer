#pragma once
#include <optional>

#include <glad.h>

class Sampler
{
private:
	GLuint gl_name;

	Sampler(GLuint gl_name);

public:
	Sampler();
	Sampler(const Sampler&) = delete;
	Sampler(Sampler&&) noexcept;
	~Sampler();

	Sampler& operator=(Sampler&);

	static std::optional<Sampler> create();

	void set_min_filtering(GLenum filter_mode) const;
	void set_mag_filtering(GLenum filter_mode) const;

	void set_wrapping(GLenum wrap_mode) const;

	void bind(GLuint texture_unit) const;
};
