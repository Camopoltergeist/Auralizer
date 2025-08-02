#include "Window.hpp"

Window::Window(SDL_Window* window) : window(window) { }

Window::Window(Window&& other) noexcept : window(nullptr)
{
	window = other.window;
	other.window = nullptr;
}

Window& Window::operator=(Window&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}

	window = other.window;
	other.window = nullptr;

	return *this;
}

Window::~Window()
{
	if(window != nullptr) {
		SDL_DestroyWindow(window);
	}
}

std::optional<Window> Window::create()
{
	constexpr SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

	SDL_Window* main_window = SDL_CreateWindow("Auralizer", 1280, 720, window_flags);

	if (main_window == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
		return std::nullopt;
	}

	return std::make_optional<Window>(Window(main_window));
}

SDL_Window* Window::get_window_ptr() const
{
	return window;
}

WindowSize Window::get_window_size() const
{
	WindowSize size{};

	SDL_GetWindowSize(window, &size.width, &size.height);

	return size;
}

void Window::swap_window() const
{
	SDL_GL_SwapWindow(window);
}
