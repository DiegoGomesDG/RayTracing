#ifndef RAYTRACING_INPUTEVENTS_H
#define RAYTRACING_INPUTEVENTS_H
#include <format>

#include "Event.h"

namespace Core {
    class KeyEvent : public Event {
    public:
        inline int get_key_code() const { return m_key_code; }
    protected:
        KeyEvent(int keycode) : m_key_code(keycode) {}
        int m_key_code;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, bool is_repeat)
            : KeyEvent(keycode), m_is_repeat(is_repeat) {}
        inline bool is_repeat() const { return m_is_repeat; }

        std::string to_string() const override {
            return std::format("KeyPressedEvent: {} (repeat = {})", m_key_code, m_is_repeat);
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        bool m_is_repeat;
    };

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(double x, double y) : m_mouseX(x), m_mouseY(y) {}

        inline double get_x() const { return m_mouseX; }
        inline double get_y() const { return m_mouseY; }

        std::string to_string() const override {
            return std::format("MouseMovedEvent: {}, {}", m_mouseX, m_mouseY);
        }
        EVENT_CLASS_TYPE(MouseMoved)
    private:
        double m_mouseX;
        double m_mouseY;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        std::string to_string() const override {
            return std::format("MouseButtonPressedEvent: {}", m_button);
        }
        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        std::string to_string() const override {
            return std::format("MouseButtonReleasedEvent: {}", m_button);
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}

#endif //RAYTRACING_INPUTEVENTS_H