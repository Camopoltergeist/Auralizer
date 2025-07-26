#pragma once
#include <optional>
#include <string>

#include <glad.h>

class Shader
{
private:
	GLuint gl_name;

	Shader(GLuint gl_name);

	static bool check_shader_compilation(GLuint shader_name);
	static bool check_program_link_status(GLuint program_name);

public:
	Shader();
	Shader(const Shader&) = delete;
	Shader(Shader&&) noexcept;
	~Shader();

	Shader& operator=(Shader& other);

	static std::optional<Shader> create(GLenum shader_type, const std::string& shader_source);

	GLuint name() const;
};

