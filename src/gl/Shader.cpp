#include "Shader.h"

#include <SDL3/SDL.h>

Shader::Shader(GLuint gl_name) : gl_name(gl_name) { }

bool Shader::check_shader_compilation(GLuint shader_name)
{
	GLint compile_status = 0;
	glGetShaderiv(shader_name, GL_COMPILE_STATUS, &compile_status);

	if (compile_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetShaderiv(shader_name, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetShaderInfoLog(shader_name, info_log.size(), nullptr, info_log.data());

		SDL_Log("Shader failed to compile:\n%s", info_log.c_str());

		return false;
	}

	return true;
}

bool Shader::check_program_link_status(GLuint program_name)
{
	GLint link_status = 0;
	glGetProgramiv(program_name, GL_LINK_STATUS, &link_status);

	if (link_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetProgramiv(program_name, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetProgramInfoLog(program_name, info_log.size(), nullptr, info_log.data());

		SDL_Log("Program failed to link:\n%s", info_log.c_str());

		return false;
	}
	
	return true;
}

Shader::Shader(Shader&& other) noexcept : gl_name(0)
{
	gl_name = other.gl_name;
	other.gl_name = 0;
}

Shader::~Shader()
{
	if (gl_name != 0) {
		glDeleteProgram(gl_name);
	}
}

Shader& Shader::operator=(Shader& other)
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteProgram(gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	return *this;
}

std::optional<Shader> Shader::create(GLenum shader_type, const std::string& shader_source)
{
	GLuint shader_name = glCreateShader(shader_type);

	if (shader_name == 0) {
		SDL_Log("Failed to create shader");
		return std::optional<Shader>();
	}

	const char* source_ptr = shader_source.c_str();

	glShaderSource(shader_name, 1, &source_ptr, nullptr);
	glCompileShader(shader_name);

	if (!Shader::check_shader_compilation(shader_name)) {
		glDeleteShader(shader_name);
		return std::optional<Shader>();
	}

	GLuint program_name = glCreateProgram();

	if (program_name == 0) {
		SDL_Log("Failed to create shader program");
		glDeleteShader(shader_name);
		return std::optional<Shader>();
	}

	glProgramParameteri(program_name, GL_PROGRAM_SEPARABLE, GL_TRUE);

	glAttachShader(program_name, shader_name);
	glLinkProgram(program_name);

	if (!Shader::check_program_link_status(program_name)) {
		glDeleteShader(shader_name);
		glDeleteProgram(program_name);

		return std::optional<Shader>();
	}

	glDeleteShader(shader_name);

	return std::make_optional<Shader>(program_name);
}
