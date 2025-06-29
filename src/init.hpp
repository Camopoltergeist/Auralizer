#pragma once

bool init_video_subsystem();
bool init_window(AppState* app_state);
bool init_opengl(AppState* app_state);
void init_imgui(AppState* app_state);
bool init_audio_subsystem();
bool init_audio_device(AppState* app_state);