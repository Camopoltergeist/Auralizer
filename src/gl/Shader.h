#pragma once
#include <optional>
#include <string>
#include <map>

#include <glad.h>

class Shader
{
private:
	GLuint gl_name;
	std::map<std::string, GLint> uniform_locations;

	explicit Shader(GLuint gl_name);

	static bool check_shader_compilation(GLuint shader_name);
	static bool check_program_link_status(GLuint program_name);

	void populate_uniforms();

public:
	Shader();
	Shader(const Shader&) = delete;
	Shader(Shader&&) noexcept;
	~Shader();

	Shader& operator=(Shader&& other) noexcept;

	static std::optional<Shader> create(GLenum shader_type, const std::string& shader_source);

	[[nodiscard]] GLuint name() const;
};

