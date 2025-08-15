#pragma once
#include <optional>

#include <SDL3/SDL.h>
#include <glad.h>

struct WindowSize {
	int width;
	int height;
};

class Window {
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context = nullptr;
	SDL_Rect restore_rectangle{};
	bool restore_maximized = false;
	bool is_fullscreen = false;

	Window(SDL_Window* window, SDL_GLContext gl_context);

	static std::optional<SDL_GLContext> create_context(SDL_Window* window);
	static void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

public:
	Window() = default;
	Window(const Window&) = delete;
	Window(Window&& other) noexcept;

	Window& operator=(Window&& other) noexcept;

	~Window();

	static std::optional<Window> create();

	[[nodiscard]] SDL_Window* get_window_ptr() const;
	[[nodiscard]] SDL_GLContext get_gl_context() const;
	[[nodiscard]] WindowSize get_window_size() const;

	void swap_window() const;

	void set_fullscreen();
	void set_windowed();

	[[nodiscard]] bool get_fullscreen() const;
};
