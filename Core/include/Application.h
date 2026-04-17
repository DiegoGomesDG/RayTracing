#ifndef RAYTRACING_APPLICATION_H
#define RAYTRACING_APPLICATION_H

#include "Layer.h"
#include "Window.h"
#include "Event.h"

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>


namespace Core {

    struct ApplicationSpecification {
        std::string name = "Application";
        WindowSettings settings;
    };

    class Application {
    public:
        Application(const ApplicationSpecification& spec = ApplicationSpecification());
        ~Application();

        void run();
        void stop();

        void raise_event(Event& event);

        template<typename TLayer>
        requires(std::is_base_of_v<TLayer, TLayer>)
        void push_layer() {
            m_layer_stack.push_back(std::make_unique<TLayer>());
        }

        template<typename TLayer>
        requires(std::is_base_of_v<TLayer, TLayer>)
        TLayer* get_layer() {
            for (const auto &layer : m_layer_stack) {
                // Warning: not the best solution!
                if (auto casted = dynamic_cast<TLayer *>(layer.get())) {
                    return casted;
                }
            }
            return nullptr;
        }

        glm::vec2 get_framebuffer_size() const;

        std::shared_ptr<Window> get_window() const { return m_window; }

        static Application& get();
        static float get_time();

    private:
        ApplicationSpecification    m_spec;
        std::shared_ptr<Window>     m_window;
        bool                        m_running = false;

        friend class Layer;

        using LayerStack = std::vector<std::unique_ptr<Layer>>;
        LayerStack m_layer_stack;
    };
}




#endif //RAYTRACING_APPLICATION_H