#include "Layer.h"

#include "Application.h"

namespace Core {
    void Layer::queue_transition(std::unique_ptr<Layer> to_layer) {
        // TODO: fix this
        auto& layer_stack = Application::get().m_layer_stack;
        for (auto& layer : layer_stack) {
            if (layer.get() == this) {
                layer = std::move(to_layer);
                return;
            }
        }
    }
}
