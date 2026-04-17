#ifndef RAYTRACING_WINDOW_H
#define RAYTRACING_WINDOW_H

#include "Event.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <functional>

namespace Core {

    struct WindowSettings {
        std::string title;
        uint32_t    width       = 1280;
        uint32_t    height      = 720;
        bool        isResizable = true;
        bool        VSync       = true;

        using EventCallbackFunction = std::function<void(Event&)>;
        EventCallbackFunction event_callback;
    };

    class Window {
    public:
        Window(const WindowSettings& settings = WindowSettings());
        ~Window();

        void create();
        void destroy();

        void update();

        void raise_event(Event& event);

        glm::vec2 framebuffer_size() const;
        glm::vec2 mouse_position() const;

        bool should_close();

        GLFWwindow* get_handle() const { return m_handle; }
    private:
        WindowSettings m_settings;
        GLFWwindow* m_handle = nullptr;
    };

}

#endif //RAYTRACING_WINDOW_H