#include "Shader.h"

#include <stdexcept>
#include <vector>

#include <SDL3/SDL.h>

Shader::Shader(GLuint gl_name) : gl_name(gl_name) { }

bool Shader::check_shader_compilation(const GLuint shader_name)
{
	GLint compile_status = 0;
	glGetShaderiv(shader_name, GL_COMPILE_STATUS, &compile_status);

	if (compile_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetShaderiv(shader_name, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetShaderInfoLog(shader_name, static_cast<GLsizei>(info_log.size()), nullptr, info_log.data());

		SDL_Log("Shader failed to compile:\n%s", info_log.c_str());

		return false;
	}

	return true;
}

bool Shader::check_program_link_status(const GLuint program_name)
{
	GLint link_status = 0;
	glGetProgramiv(program_name, GL_LINK_STATUS, &link_status);

	if (link_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetProgramiv(program_name, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetProgramInfoLog(program_name, static_cast<GLsizei>(info_log.size()), nullptr, info_log.data());

		SDL_Log("Program failed to link:\n%s", info_log.c_str());

		return false;
	}

	return true;
}

void Shader::populate_uniforms() {
	GLint active_uniforms = 0;

	glGetProgramiv(gl_name, GL_ACTIVE_UNIFORMS, &active_uniforms);

	for(unsigned int i = 0; i < active_uniforms; i++) {
		GLint name_length = 0;

		glGetActiveUniformsiv(gl_name, 1, &i, GL_UNIFORM_NAME_LENGTH, &name_length);

		std::string buffer(name_length, ' ');

		GLsizei _length = 0;
		GLint _size = 0;
		GLenum _type = 0;

		glGetActiveUniform(gl_name, i, static_cast<GLsizei>(buffer.size()), &_length, &_size, &_type, buffer.data());

		// Remove the extra (and unneeded at this point) null character from the end
		buffer.pop_back();

		const GLint location = glGetUniformLocation(gl_name, buffer.c_str());

		if(location == -1) {
			SDL_Log("Failed to get uniform location for \"%s\"", buffer.c_str());
			continue;
		}

		uniform_locations[buffer] = location;
	}
}

Shader::Shader()
{
	gl_name = 0;
}

Shader::Shader(Shader&& other) noexcept : gl_name(0)
{
	gl_name = other.gl_name;
	other.gl_name = 0;

	uniform_locations = std::move(other.uniform_locations);
}

Shader::~Shader()
{
	if (gl_name != 0) {
		glDeleteProgram(gl_name);
	}
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteProgram(gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	uniform_locations = std::move(other.uniform_locations);

	return *this;
}

std::optional<Shader> Shader::create(const GLenum shader_type, const std::string& shader_source)
{
	const GLuint shader_name = glCreateShader(shader_type);

	if (shader_name == 0) {
		SDL_Log("Failed to create shader");
		return std::nullopt;
	}

	const char* source_ptr = shader_source.c_str();

	glShaderSource(shader_name, 1, &source_ptr, nullptr);
	glCompileShader(shader_name);

	if (!Shader::check_shader_compilation(shader_name)) {
		glDeleteShader(shader_name);
		return std::nullopt;
	}

	const GLuint program_name = glCreateProgram();

	if (program_name == 0) {
		SDL_Log("Failed to create shader program");
		glDeleteShader(shader_name);
		return std::nullopt;
	}

	glProgramParameteri(program_name, GL_PROGRAM_SEPARABLE, GL_TRUE);

	glAttachShader(program_name, shader_name);
	glLinkProgram(program_name);

	if (!Shader::check_program_link_status(program_name)) {
		glDeleteShader(shader_name);
		glDeleteProgram(program_name);

		return std::nullopt;
	}

	glDeleteShader(shader_name);

	Shader shader = Shader(program_name);
	shader.populate_uniforms();

	return std::make_optional<Shader>(std::move(shader));
}

GLuint Shader::name() const
{
	return gl_name;
}

std::optional<GLint> Shader::get_uniform_location(const std::string& uniform_name) const {
	GLint location = 0;

	try {
		location = uniform_locations.at(uniform_name);
	}
	catch (const std::out_of_range& e) {
		return std::nullopt;
	}

	return std::make_optional(location);
}

