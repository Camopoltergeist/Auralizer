#pragma once
#include <optional>

#include <glad.h>

class Sampler
{
private:
	GLuint gl_name;

	explicit Sampler(GLuint gl_name);

public:
	Sampler();
	Sampler(const Sampler&) = delete;
	Sampler(Sampler&&) noexcept;
	~Sampler();

	Sampler& operator=(Sampler&&) noexcept;

	static std::optional<Sampler> create();

	void set_min_filtering(GLint filter_mode) const;
	void set_mag_filtering(GLint filter_mode) const;

	void set_wrapping(GLint wrap_mode) const;

	void bind(GLuint texture_unit) const;
};
