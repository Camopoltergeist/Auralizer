#pragma once
#include <optional>

#include <glad.h>

#include "Shader.h"

class Pipeline
{
private:
	GLuint gl_name;

	explicit Pipeline(GLuint gl_name);

	static bool validate_pipeline(GLuint pipeline_name);

public:
	Pipeline();
	Pipeline(const Pipeline&) = delete;
	Pipeline(Pipeline&&) noexcept;
	~Pipeline();

	Pipeline& operator=(Pipeline&& other) noexcept;

	static std::optional<Pipeline> create(const Shader& vertex_shader, const Shader& fragment_shader);

	[[nodiscard]] GLuint name() const;

	void bind() const;
};

