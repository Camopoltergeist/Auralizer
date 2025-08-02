#include <glad.h>
#include <SDL3/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "AppState.hpp"
#include "miniaudio.h"
#include "AnalysisNode.hpp"

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

	SDL_Window* main_window = SDL_CreateWindow("Auralizer", 1280, 720, window_flags);

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

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(debug_message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	SDL_GL_SetSwapInterval(1);

	return true;
}

void init_imgui(AppState* app_state) {
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.f, 0.f, 0.f, 0.5f);

	ImGui_ImplSDL3_InitForOpenGL(app_state->main_window, app_state->gl_context);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	ImFontConfig font_config;
	font_config.MergeMode = true;

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.Fonts->AddFontFromFileTTF("./fonts/Noto_Sans/NotoSans-Regular.ttf");
	io.Fonts->AddFontFromFileTTF("./fonts/Noto_Sans_JP/NotoSansJP-Regular.ttf", 0.0, &font_config);
	io.Fonts->AddFontFromFileTTF("./fonts/Noto_Sans_KR/NotoSansKR-Regular.ttf", 0.0, &font_config);
	io.Fonts->AddFontFromFileTTF("./fonts/Noto_Sans_SC/NotoSansSC-Regular.ttf", 0.0, &font_config);
	io.Fonts->AddFontFromFileTTF("./fonts/Noto_Sans_TC/NotoSansTC-Regular.ttf", 0.0, &font_config);
	io.Fonts->Build();
}

bool init_audio(AppState* app_state) {
	ma_result result;
	ma_engine* audio_engine = new ma_engine();

	result = ma_engine_init(nullptr, audio_engine);

	if (result != MA_SUCCESS) {
		SDL_Log("Failed to initialize audio engine");
		return false;
	}

	app_state->audio_engine = audio_engine;
	ma_engine_set_volume(app_state->audio_engine, app_state->audio_volume);

	auto analysis_node_opt = AnalysisNode::create(ma_engine_get_node_graph(app_state->audio_engine), 2048, 2);

	if (!analysis_node_opt) {
		return false;
	}

	ma_node_attach_output_bus(analysis_node_opt.get(), 0, ma_engine_get_endpoint(app_state->audio_engine), 0);

	app_state->analysis_node = std::move(analysis_node_opt);

	return true;
}
