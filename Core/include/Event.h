#ifndef RAYTRACING_EVENT_H
#define RAYTRACING_EVENT_H
#include <functional>
#include <string>

namespace Core {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MoveMoved, MouseScrolled
    };

    #define EVENT_CLASS_TYPE(type) static EventType get_event_type() { return EventType::type; }\
        virtual EventType get_event_type() const override { return GetStaticType(); }\
        virtual const char* get_name() const override { return #type; }

    class Event {
    public:
        bool handled = false;

        virtual ~Event() {}
        virtual EventType get_event_type() const = 0;
        virtual const char* get_name() const = 0;
        virtual std::string to_string() const { return get_name(); }
    };

    class EventDispatcher {
        template<typename T>
        using EventHandler = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event) : m_event(event) {}

        template<typename T>
        bool dispatch(EventHandler<T> function) {
            if (m_event.get_event_type() == T::GetStaticType() && !m_event.handled) {
                m_event.handled = function(*(T*)&m_event);
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };
}

#endif //RAYTRACING_EVENT_H