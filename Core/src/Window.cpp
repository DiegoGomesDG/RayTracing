#include "Window.h"

#include <glad/glad.h>

#include <iostream>
#include <ostream>
#include <cassert>

namespace Core {
    Window::Window(const WindowSettings &settings)
        : m_settings(settings) {
    }

    Window::~Window() {
        destroy();
    }

    void Window::create() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        m_handle = glfwCreateWindow(
            m_settings.width,
            m_settings.height,
            m_settings.title.c_str(),
            nullptr,
            nullptr
        );

        if (m_handle == nullptr) {
            std::cerr << "Failed to create GLFW window.\n" << std::endl;
            glfwTerminate();
            assert(false);
        }

        glfwMakeContextCurrent(m_handle);

        glfwSwapInterval(m_settings.VSync ? 1 : 0);


    }

    void Window::destroy() {
        if (m_handle != nullptr) {
            glfwDestroyWindow(m_handle);
        }
        m_handle = nullptr;
    }

    void Window::update() {
        glfwSwapBuffers(m_handle);
    }

    void Window::raise_event(Event &event) {
        if (m_settings.event_callback) {
            m_settings.event_callback(event);
        }
    }

    glm::vec2 Window::framebuffer_size() const {
        int width, height;
        glfwGetWindowSize(m_handle, &width, &height);
        return {width, height};
    }

    glm::vec2 Window::mouse_position() const {
        double x, y;
        glfwGetCursorPos(m_handle, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    bool Window::should_close() {
        return glfwWindowShouldClose(m_handle);
    }
}
