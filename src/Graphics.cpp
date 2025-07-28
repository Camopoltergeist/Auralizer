#include "Graphics.h"

#include <string>
#include <sstream>
#include <fstream>

#include <SDL3/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Graphics::Graphics()
{
}

Graphics::Graphics(
	VertexArray vertex_array,
	GLBuffer vertex_buffer,
	GLBuffer index_buffer,
	Shader vertex_shader,
	Shader fragment_shader,
	Pipeline pipeline,
	Texture texture,
	Sampler sampler
) :
	vertex_array(std::move(vertex_array)),
	vertex_buffer(std::move(vertex_buffer)),
	index_buffer(std::move(index_buffer)),
	vertex_shader(std::move(vertex_shader)),
	fragment_shader(std::move(fragment_shader)),
	pipeline(std::move(pipeline)),
	texture(std::move(texture)),
	sampler(std::move(sampler))
{ }

std::string load_text_file(const std::string& file_path) {
	std::ifstream file(file_path);
	std::ostringstream sstream;

	sstream << file.rdbuf();

	return sstream.str();
}

std::optional<Graphics> Graphics::init()
{
	auto vertex_array_opt = VertexArray::create();

	if (!vertex_array_opt.has_value()) {
		return std::optional<Graphics>();
	}

	VertexArray vertex_array = std::move(vertex_array_opt.value());
	vertex_array.enable_array_attrib(0);
	vertex_array.enable_array_attrib(1);

	vertex_array.set_attrib_format(0, 2, GL_FLOAT, GL_FALSE, 0);
	vertex_array.set_attrib_format(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2);

	std::vector<GLfloat> vertices = {
		-1.f, -1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
		-1.f, 1.f, 0.f, 1.f
	};

	auto vertex_buffer_opt = GLBuffer::create(vertices);

	if (!vertex_buffer_opt.has_value()) {
		return std::optional<Graphics>();
	}

	GLBuffer vertex_buffer = std::move(vertex_buffer_opt.value());

	vertex_array.bind_buffer_to_binding_index(0, vertex_buffer, 0, sizeof(float) * 4);
	vertex_array.bind_attrib_to_binding_index(0, 0);
	vertex_array.bind_attrib_to_binding_index(0, 1);

	std::vector<GLuint> indices = {
		0, 1, 2,
		2, 3, 0
	};

	auto index_buffer_opt = GLBuffer::create(indices);

	if (!index_buffer_opt.has_value()) {
		return std::optional<Graphics>();
	}

	vertex_array.bind_element_buffer(index_buffer_opt.value());

	std::string vertex_source = load_text_file("./shaders/vertex.glsl");
	std::string fragment_source = load_text_file("./shaders/fragment.glsl");

	auto vertex_shader_opt = Shader::create(GL_VERTEX_SHADER, vertex_source);
	auto fragment_shader_opt = Shader::create(GL_FRAGMENT_SHADER, fragment_source);

	if (!vertex_shader_opt.has_value() || !fragment_shader_opt.has_value()) {
		return std::optional<Graphics>();
	}

	auto pipeline_opt = Pipeline::create(vertex_shader_opt.value(), fragment_shader_opt.value());

	if (!pipeline_opt.has_value()) {
		return std::optional<Graphics>();
	}

	auto sampler_opt = Sampler::create();

	if (!sampler_opt.has_value()) {
		return std::optional<Graphics>();
	}

	sampler_opt.value().set_min_filtering(GL_LINEAR_MIPMAP_LINEAR);

	int width = 0;
	int height = 0;
	int channel_count = 0;

	const char* image_path = "test.png";

	unsigned char* texture_data = stbi_load(image_path, &width, &height, &channel_count, 4);

	if (texture_data == nullptr) {
		SDL_Log("Failed to load image at \"%s\": %s", image_path, stbi_failure_reason());
		return std::optional<Graphics>();
	}

	auto texture_opt = Texture::create(width, height, GL_RGBA8);

	if (!texture_opt.has_value()) {
		stbi_image_free(texture_data);
		return std::optional<Graphics>();
	}

	texture_opt.value().upload_texture(GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	texture_opt.value().generate_mipmap();

	return std::make_optional<Graphics>(
		std::move(vertex_array),
		std::move(vertex_buffer),
		std::move(index_buffer_opt.value()),
		std::move(vertex_shader_opt.value()),
		std::move(fragment_shader_opt.value()),
		std::move(pipeline_opt.value()),
		std::move(texture_opt.value()),
		std::move(sampler_opt.value())
	);
}

Graphics::~Graphics()
{
}
