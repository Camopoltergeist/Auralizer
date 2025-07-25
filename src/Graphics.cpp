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

	std::vector<float> vertices = {
		-1.f, -1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
		-1.f, 1.f, 0.f, 1.f
	};

	auto vertex_buffer_opt = GLBuffer::create(vertices);

	std::vector<GLuint> indices = {
		0, 1, 2,
		2, 3, 0
	};

	auto index_buffer_opt = GLBuffer::create(indices);

	return std::make_optional<Graphics>(std::move(vertex_array_opt.value()), std::move(vertex_buffer_opt.value()), std::move(index_buffer_opt.value()));
}

Graphics::~Graphics()
{
}
