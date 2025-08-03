#include <glad.h>
#include <SDL3/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "AppState.hpp"
#include "miniaudio.h"
#include "AnalyserNode.hpp"

bool init_video_subsystem() {
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL Video subsystem: %s", SDL_GetError());
		return false;
	}

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

	ImGui_ImplSDL3_InitForOpenGL(app_state->main_window.get_window_ptr(), app_state->main_window.get_gl_context());
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

	auto analysis_node_opt = AnalyserNode::create(ma_engine_get_node_graph(app_state->audio_engine), 2048, 2);

	if (!analysis_node_opt) {
		return false;
	}

	ma_node_attach_output_bus(analysis_node_opt.get(), 0, ma_engine_get_endpoint(app_state->audio_engine), 0);

	app_state->analysis_node = std::move(analysis_node_opt);

	auto capture_device = CaptureDevice::create();

	if(!capture_device) {
		return false;
	}

	capture_device->start();

	app_state->capture_device = std::move(capture_device);

	return true;
}
