#include "Graphics.h"

#include <SDL3/SDL.h>

Graphics::Graphics(VertexArray vertex_array, GLBuffer vertex_buffer, GLBuffer index_buffer) :
	vertex_array(std::move(vertex_array)),
	vertex_buffer(std::move(vertex_buffer)),
	index_buffer(std::move(index_buffer))
{

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

	std::vector<float> vertices = {
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

	return std::make_optional<Graphics>(std::move(vertex_array_opt.value()), std::move(vertex_buffer_opt.value()), std::move(index_buffer_opt.value()));
}

Graphics::~Graphics()
{
}
