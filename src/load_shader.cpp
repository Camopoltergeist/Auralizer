#include <glad.h>
#include <SDL3/SDL.h>

#include <string>
#include <sstream>
#include <fstream>

GLuint load_shader(std::string filepath, GLenum shader_type) {
	std::ifstream file(filepath);
	std::ostringstream sstream;

	sstream << file.rdbuf();

	std::string shader_source = sstream.str();
	const char* source_ptr = shader_source.c_str();
	const GLint source_size = static_cast<int>(shader_source.size());

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source_ptr, &source_size);

	glCompileShader(shader);

	GLint compile_status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

	if (compile_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetShaderInfoLog(shader, static_cast<GLsizei>(info_log.size()), nullptr, info_log.data());

		SDL_Log("Shader failed to compile:\n%s", info_log.c_str());
		glDeleteShader(shader);

		return 0;
	}

	GLuint program = glCreateProgram();
	glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	glAttachShader(program, shader);
	glLinkProgram(program);

	GLint link_status = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);

	if (link_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetProgramInfoLog(program, static_cast<GLsizei>(info_log.size()), nullptr, info_log.data());

		SDL_Log("Program failed to link:\n%s", info_log.c_str());
		glDeleteShader(shader);
		glDeleteProgram(program);

		return 0;
	}

	glDeleteShader(shader);

	return program;
}