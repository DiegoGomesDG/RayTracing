#ifndef RAYTRACING_WINDOWEVENTS_H
#define RAYTRACING_WINDOWEVENTS_H

#include "Event.h"

#include <format>

namespace Core {
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}
        EVENT_CLASS_TYPE(WindowClose)
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

        inline uint32_t get_width() const { return m_width; }
        inline uint32_t get_height() const { return m_height; }

        std::string to_string() const override {
            return std::format("WindowResizeEvent: {} {}", m_width, m_height);
        }
        EVENT_CLASS_TYPE(WindowResize)
    private:
        uint32_t m_width;
        uint32_t m_height;
    };
}

#endif //RAYTRACING_WINDOWEVENTS_H