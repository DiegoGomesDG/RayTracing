cmake_minimum_required(VERSION 3.20)
project(RayTracingProject)

set(CMAKE_CXX_STANDARD 20)

# Third-party libraries
# GLFW
add_subdirectory(thirdparty/glfw)

# GLAD
add_library(glad STATIC
        thirdparty/glad/src/glad.c
)
target_include_directories(glad PUBLIC
        thirdparty/glad/include
)

# ImGui (submodule)
add_library(imgui STATIC)

target_sources(imgui PRIVATE
        thirdparty/imgui/imgui.cpp
        thirdparty/imgui/imgui_draw.cpp
        thirdparty/imgui/imgui_tables.cpp
        thirdparty/imgui/imgui_widgets.cpp

        thirdparty/imgui/backends/imgui_impl_glfw.cpp
        thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        thirdparty/imgui
        thirdparty/imgui/backends
)

# Link GLFW and ImGui
target_link_libraries(imgui
        PUBLIC glfw glad
)

# Modules
add_subdirectory(Core)
add_subdirectory(RayTracing)