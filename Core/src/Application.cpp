#include "Application.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cassert>
#include <iostream>
#include <ranges>

namespace Core {

    static Application* s_application = nullptr;

    static void GLFW_error_callback(int error, const char* description) {
        std::cerr << "[GLFW Error]: " << description << std::endl;
    }

    Application::Application(const ApplicationSpecification& spec)
        : m_spec(spec) {

        s_application = this;

        glfwSetErrorCallback(GLFW_error_callback);

        if (!glfwInit()) {
            std::cerr << "Failed to initialize" << std::endl;
            return;
        }

        if (m_spec.settings.title.empty()) {
            m_spec.settings.title = m_spec.name;
        }

        m_spec.settings.event_callback = [this](Event& event) {
            raise_event(event);
        };

        m_window = std::make_shared<Window>(m_spec.settings);
        m_window->create();
    }

    Application::~Application() {
        m_window->destroy();
        glfwTerminate();
        s_application = nullptr;
    }

    void Application::run() {
        m_running = true;
        float last_time = get_time();

        while (m_running) {
            glfwPollEvents();

            if (m_window->should_close()) {
                stop();
                break;
            }

            float current_time = get_time();
            float timestep = glm::clamp(current_time - last_time, 0.001f, 0.1f);
            last_time = current_time;

            // Main layer update
            for (const std::unique_ptr<Layer>& layer : m_layer_stack) {
                layer->on_update(timestep);
            }

            glViewport(
                0,
                0,
                m_window->framebuffer_size().x,
                m_window->framebuffer_size().y
            );

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            for (const std::unique_ptr<Layer>& layer : m_layer_stack) {
                layer->on_render();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            m_window->update();
        }
    }

    void Application::stop() {
        m_running = false;
    }

    void Application::raise_event(Event &event) {
        for (auto& layer : std::views::reverse(m_layer_stack)) {
            layer->on_event(event);
        }
    }

    glm::vec2 Application::get_framebuffer_size() const {
        return m_window->framebuffer_size();
    }

    Application& Application::get() {
        assert(s_application);
        return *s_application;
    }

    float Application::get_time() {
        return static_cast<float>(glfwGetTime());
    }
}
