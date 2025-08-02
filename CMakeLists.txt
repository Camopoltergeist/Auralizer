cmake_minimum_required(VERSION 3.29)
project(Auralizer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory("libs/SDL/")
add_subdirectory("libs/miniaudio")
include_directories("libs/glad/" "libs/imgui" "libs/miniaudio" "libs/stb" "libs/fftw3/win32-x64")

add_library(fftw3 SHARED IMPORTED)
set_target_properties(fftw3 PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_SOURCE_DIR}/libs/fftw3/win32-x64/"
    IMPORTED_LOCATION_RELEASE "${PROJECT_SOURCE_DIR}/libs/fftw3/win32-x64/"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/libs/fftw3/win32-x64/"
    IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/libs/fftw3/win32-x64/libfftw3f-3.lib"
)

add_executable(Auralizer src/main.cpp
        libs/imgui/imconfig.h
        libs/imgui/imgui.cpp
        libs/imgui/imgui.h
        libs/imgui/imgui_demo.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_internal.h
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/imstb_rectpack.h
        libs/imgui/imstb_textedit.h
        libs/imgui/imstb_truetype.h
        libs/imgui/backends/imgui_impl_sdl3.cpp
        libs/imgui/backends/imgui_impl_sdl3.h
        libs/imgui/backends/imgui_impl_opengl3.h
        libs/imgui/backends/imgui_impl_opengl3.cpp
        src/AnalysisNode.cpp
        src/AnalysisNode.hpp
        src/AppState.hpp
        src/AppState.cpp
        src/Graphics.cpp
        src/Graphics.h
        src/GUI.cpp
        src/GUI.hpp
        src/init.cpp
        src/init.hpp
        src/load_shader.cpp
        src/load_shader.hpp
        src/RollingBuffer.hpp
        src/gl/GLBuffer.cpp
        src/gl/GLBuffer.h
        src/gl/Pipeline.cpp
        src/gl/Pipeline.h
        src/gl/Sampler.cpp
        src/gl/Sampler.h
        src/gl/Shader.cpp
        src/gl/Shader.h
        src/gl/Texture.cpp
        src/gl/Texture.h
        src/gl/VertexArray.cpp
        src/gl/VertexArray.h
        libs/glad/glad.c)

target_link_libraries(Auralizer SDL3-shared miniaudio fftw3)
