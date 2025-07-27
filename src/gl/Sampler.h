#pragma once


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
};

