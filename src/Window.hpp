#pragma once
#include <optional>

#include <SDL3/SDL.h>

struct WindowSize {
	int width;
	int height;
};

class Window {
	SDL_Window* window = nullptr;

	explicit Window(SDL_Window* window);
public:
	Window() = default;
	Window(const Window&) = delete;
	Window(Window&& other) noexcept;

	Window& operator=(Window&& other) noexcept;

	~Window();

	static std::optional<Window> create();

	[[nodiscard]] SDL_Window* get_window_ptr() const;
	[[nodiscard]] WindowSize get_window_size() const;

	void swap_window() const;
};
