#include <glad.h>
#include <SDL3/SDL.h>

#include "AppState.hpp"
#include "load_shader.hpp"

void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	SDL_Log("GL Message: %s", message);
}

bool init_video_subsystem() {
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL Video subsystem: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool init_window(AppState* app_state) {
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

	SDL_Window* main_window = SDL_CreateWindow("Hello Window!", 1280, 720, window_flags);

	if (main_window == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
		return false;
	}

	app_state->main_window = main_window;

	return true;
}

bool init_opengl(AppState* app_state) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext gl_context = SDL_GL_CreateContext(app_state->main_window);

	if (gl_context == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s", SDL_GetError());
		return false;
	}

	app_state->gl_context = gl_context;

	int version = gladLoadGL();

	if (version == 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OpenGL functions");
		return false;
	}

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

	glDebugMessageCallback(debug_message_callback, nullptr);
	
	SDL_GL_SetSwapInterval(1);

	GLuint vertex_array_object = 0;

	glCreateVertexArrays(1, &vertex_array_object);

	if (vertex_array_object == 0) {
		SDL_Log("Failed to create vertex array object");
		return false;
	}

	glEnableVertexArrayAttrib(vertex_array_object, 0);
	glEnableVertexArrayAttrib(vertex_array_object, 1);

	glVertexArrayAttribFormat(vertex_array_object, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vertex_array_object, 1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2);

	app_state->vertex_array_object = vertex_array_object;

	GLuint vertex_buffer = 0;

	glCreateBuffers(1, &vertex_buffer);

	if (vertex_buffer == 0) {
		SDL_Log("Failed to create vertex buffer");
		return false;
	}

	GLfloat vertices[] = {
		-1.f, -1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
		-1.f, 1.f, 0.f, 1.f
	};

	glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, 0);

	glVertexArrayVertexBuffer(vertex_array_object, 0, vertex_buffer, 0, sizeof(GLfloat) * 4);
	glVertexArrayAttribBinding(vertex_array_object, 0, 0);
	glVertexArrayAttribBinding(vertex_array_object, 1, 0);

	GLuint index_buffer = 0;

	glCreateBuffers(1, &index_buffer);

	if (index_buffer == 0) {
		SDL_Log("Failed to create index buffer");
		return false;
	}

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glNamedBufferStorage(index_buffer, sizeof(indices), indices, 0);

	glVertexArrayElementBuffer(vertex_array_object, index_buffer);

	app_state->vertex_buffer = vertex_buffer;

	GLuint vertex_shader = load_shader("./shaders/vertex.glsl", GL_VERTEX_SHADER);
	
	if (vertex_shader == 0) {
		return false;
	}

	GLuint fragment_shader = load_shader("./shaders/fragment.glsl", GL_FRAGMENT_SHADER);

	if (fragment_shader == 0) {
		return false;
	}

	app_state->vertex_shader = vertex_shader;
	app_state->fragment_shader = fragment_shader;

	GLuint pipeline = 0;
	glCreateProgramPipelines(1, &pipeline);

	glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertex_shader);
	glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragment_shader);

	app_state->pipeline = pipeline;

	return true;
}
