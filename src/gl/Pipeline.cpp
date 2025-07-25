#include "Pipeline.h"

#include <SDL3/SDL.h>

Pipeline::Pipeline(GLuint gl_name) : gl_name(gl_name) { }

bool Pipeline::validate_pipeline(GLuint pipeline_name)
{
	GLint validate_status = 0;
	glGetProgramPipelineiv(pipeline_name, GL_VALIDATE_STATUS, &validate_status);

	if (validate_status != GL_TRUE) {
		GLint info_log_length = 0;
		glGetProgramPipelineiv(pipeline_name, GL_INFO_LOG_LENGTH, &info_log_length);

		std::string info_log(info_log_length, ' ');
		glGetProgramPipelineInfoLog(pipeline_name, info_log.size(), nullptr, info_log.data());

		SDL_Log("Program pipeline failed to validate:\n%s", info_log.c_str());

		return false;
	}

	return true;
}

Pipeline::Pipeline(Pipeline&& other) noexcept
{
	gl_name = other.gl_name;
	other.gl_name = 0;
}

Pipeline::~Pipeline()
{
	if (gl_name != 0) {
		glDeleteProgramPipelines(1, &gl_name);
	}
}

Pipeline& Pipeline::operator=(Pipeline& other)
{
	if (this == &other) {
		return *this;
	}

	if (gl_name != 0) {
		glDeleteProgramPipelines(1, &gl_name);
	}

	gl_name = other.gl_name;
	other.gl_name = 0;

	return *this;
}

std::optional<Pipeline> Pipeline::create(const Shader& vertex_shader, const Shader& fragment_shader)
{
	GLuint pipeline_name = 0;

	glCreateProgramPipelines(1, &pipeline_name);

	if (pipeline_name == 0) {
		SDL_Log("Failed to create program pipeline");
		return std::optional<Pipeline>();
	}

	glUseProgramStages(pipeline_name, GL_VERTEX_SHADER_BIT, vertex_shader.name());
	glUseProgramStages(pipeline_name, GL_FRAGMENT_SHADER_BIT, fragment_shader.name());

	if (!Pipeline::validate_pipeline(pipeline_name)) {
		glDeleteProgramPipelines(1, &pipeline_name);
		return std::optional<Pipeline>();
	}

	return std::make_optional<Pipeline>(pipeline_name);
}
