#ifndef RAYTRACINGPROJECT_APPLAYER_H
#define RAYTRACINGPROJECT_APPLAYER_H

#include <thread>

#include "Layer.h"
#include "InputEvents.h"
#include "WindowEvents.h"
#include "GLImage.h"

#include "Camera.h"
#include "hittable_list.h"


class AppLayer : public Core::Layer{
public:
    AppLayer();
    virtual ~AppLayer() override;

    void on_event(Core::Event &event) override;
    void on_update(float timestamp) override;
    void on_render() override;

private:
    void init_scene();
    void start_render(int width, int height);

private:
    hittable_list   m_world;
    Camera          m_camera;

    std::shared_ptr<Core::GLImage>  m_image;
    std::vector<uint32_t>           m_framebuffer;

    std::thread         m_render_thread;
    std::atomic<bool>   m_is_rendering      = false;
    std::atomic<bool>   m_cancel_rendering  = false;

    std::mutex          m_framebuffer_mutex;
};

#endif //RAYTRACINGPROJECT_APPLAYER_H
