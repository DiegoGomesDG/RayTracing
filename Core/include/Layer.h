#ifndef RAYTRACING_LAYER_H
#define RAYTRACING_LAYER_H

#include <Event.h>

#include <memory>

namespace Core {

    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void on_event(Event& event) {}

        virtual void on_update(float timestamp) {}
        virtual void on_render() {}

        /* Create a new object of type T (which must be a Layer), pass it some arguments,
         * wrap it in a std::unique_ptr, and send it to queue_transition. */
        template<std::derived_from<Layer> T, typename... Args>
        void transition_to(Args&&... args) {
            queue_transition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
        }

    private:
        void queue_transition(std::unique_ptr<Layer> layer);
    };

}


#endif //RAYTRACING_LAYER_H