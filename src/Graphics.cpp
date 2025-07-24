#include "Graphics.h"

#include <SDL3/SDL.h>

Graphics::Graphics(VertexArray vertex_array) : vertex_array(std::move(vertex_array))
{
	vertex_array = vertex_array;
}

std::optional<Graphics> Graphics::init()
{
	auto vertex_array_opt = VertexArray::create();

	if (!vertex_array_opt.has_value()) {
		return std::optional<Graphics>();
	}

	VertexArray vertex_array = std::move(vertex_array_opt.value());

	return std::make_optional<Graphics>(std::move(vertex_array));
}

Graphics::~Graphics()
{
}
