#include "Window.hpp"

static bool is_opengl_loaded = false;

Window::Window(SDL_Window* window, SDL_GLContext gl_context) : window(window), gl_context(gl_context) { }

std::optional<SDL_GLContext> Window::create_context(SDL_Window* window)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	if (gl_context == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s", SDL_GetError());
		return std::nullopt;
	}

	if(!is_opengl_loaded) {
		if (const int version = gladLoadGL(); version == 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OpenGL functions");
			return std::nullopt;
		}
	}

	is_opengl_loaded = true;

	int opengl_major_version = 0;
	int opengl_minor_version = 0;
	int context_flags = 0;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &opengl_major_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &opengl_minor_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &context_flags);

	SDL_Log("OpenGL %i.%i", opengl_major_version, opengl_minor_version);

	if ((context_flags & SDL_GL_CONTEXT_DEBUG_FLAG) != 0) {
		SDL_Log("Debug Context");
	}

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(debug_message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	SDL_GL_SetSwapInterval(1);

	return gl_context;
}

void Window::debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	SDL_Log("GL Message: %s", message);
}

Window::Window(Window&& other) noexcept : window(nullptr), gl_context(nullptr)
{
	window = other.window;
	gl_context = other.gl_context;
	is_fullscreen = other.is_fullscreen;
	restore_rectangle = other.restore_rectangle;

	other.window = nullptr;
	other.gl_context = nullptr;
}

Window& Window::operator=(Window&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	this->~Window();

	window = other.window;
	gl_context = other.gl_context;
	is_fullscreen = other.is_fullscreen;
	restore_rectangle = other.restore_rectangle;

	other.window = nullptr;
	other.gl_context = nullptr;

	return *this;
}

Window::~Window()
{
	if(gl_context != nullptr) {
		SDL_GL_DestroyContext(gl_context);
	}

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

	const auto gl_context = Window::create_context(main_window);

	if(!gl_context.has_value()) {
		return std::nullopt;
	}

	return std::make_optional<Window>(Window(main_window, gl_context.value()));
}

SDL_Window* Window::get_window_ptr() const
{
	return window;
}

SDL_GLContext Window::get_gl_context() const
{
	return gl_context;
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

void Window::set_fullscreen()
{
	const SDL_DisplayID current_display = SDL_GetDisplayForWindow(window);
	SDL_Rect display_bounds{};

	SDL_Rect window_rect{};

	SDL_GetWindowPosition(window, &window_rect.x, &window_rect.y);
	SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);

	SDL_GetDisplayBounds(current_display, &display_bounds);

	SDL_SetWindowBordered(window, false);
	SDL_SetWindowPosition(window, display_bounds.x, display_bounds.y);
	// The + 1 prevents the window from going into exclusive fullscreen mode on the next SDL_GL_SwapWindow call
	// https://github.com/libsdl-org/SDL/issues/12791
	// https://github.com/dosbox-staging/dosbox-staging/pull/4415
	SDL_SetWindowSize(window, display_bounds.w, display_bounds.h + 1);

	is_fullscreen = true;
	restore_rectangle = window_rect;
}

void Window::set_windowed()
{
	SDL_SetWindowBordered(window, true);
	SDL_SetWindowPosition(window, restore_rectangle.x, restore_rectangle.y);
	SDL_SetWindowSize(window, restore_rectangle.w, restore_rectangle.h);

	is_fullscreen = false;
}

bool Window::get_fullscreen() const
{
	return is_fullscreen;
}
