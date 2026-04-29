#include "layers/AppLayer.h"

#include "Application.h"
#include "imgui.h"
#include "material.h"
#include "Sphere.h"

AppLayer::AppLayer() {
    init_scene();
}

AppLayer::~AppLayer() {
    m_cancel_rendering = true;

    if (m_render_thread.joinable()) {
        m_render_thread.join();
    }
}

void AppLayer::on_event(Core::Event &event) {
    // Layer::on_event(event);
}

void AppLayer::on_update(float timestamp) {
    // Layer::on_update(timestamp);
}

void AppLayer::on_render() {
    ImGuiViewport* vp = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("Root", nullptr, flags);

    float sidebarWidth = 340.0f;
    ImVec2 avail = ImGui::GetContentRegionAvail();

    // Left: Viewport
    ImVec2 size;
    ImGui::BeginChild("Viewport", ImVec2(avail.x - sidebarWidth, avail.y), true);
    size = ImGui::GetContentRegionAvail();

    std::lock_guard<std::mutex> lock(m_framebuffer_mutex);
    if (!m_framebuffer.empty()) {
        int width  = static_cast<int>(size.x);
        int height = static_cast<int>(size.y);

        if (!m_image) {
            m_image = std::make_shared<Core::GLImage>(width, height);
        } else if (m_image->get_width() != width || m_image->get_height() != height) {
            m_image->resize(width, height);
        }
        m_image->set_data(m_framebuffer.data());
    }

    if (m_image) {
        ImGui::Image(
            m_image->get_imgui_texture_id(),
            size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    } else {
        ImGui::Text("No image rendered");
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Settings
    ImGui::BeginChild("Settings", ImVec2(sidebarWidth, avail.y), true);

    float button_area_height = 130.0f;

    // Top: Settings
    ImGui::BeginChild("SettingsArea", ImVec2(0, avail.y - button_area_height), true);

    if (ImGui::CollapsingHeader("Render##render_header")) {
        ImGui::SliderInt("Samples", &m_camera.samples_per_pixel, 1, 500);
        ImGui::SliderInt("Max Depth", &m_camera.max_depth, 1, 100);

        const char* items[] = { "Shaded", "Normals" };
        static int mode = 0;

        if (ImGui::Combo("Render Mode", &mode, items, IM_ARRAYSIZE(items))) {
            m_camera.render_mode = (mode == 1) ? Camera::RenderMode::Normals : Camera::RenderMode::Shaded;
        }
    }

    if (ImGui::CollapsingHeader("Camera")) {

        ImGui::DragFloat3("Look From", &m_camera.lookfrom.e[0], 1.00f);
        ImGui::DragFloat3("Look At", &m_camera.lookat.e[0], 1.00f);

        auto vfov = static_cast<float>(m_camera.vfov);
        if (ImGui::SliderFloat("Vertical FOV", &vfov, 1.0f, 179.0f, "%.1f")) {
            m_camera.vfov = static_cast<real>(vfov);
        }

        ImGui::SliderFloat("Focal Distance", &m_camera.focus_dist, 1.00f, 200.0f, "%.1f");


    }
    ImGui::EndChild();

    ImGui::Spacing();

    float progress = m_render_progress.load();

    float eta = 0.0f;
    if (progress > 0.001f) {
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - m_render_start).count();
        eta = elapsed * (1.0f - progress) / progress;
    }
    ImGui::BeginChild("RenderArea", ImVec2(0, 0), true);
    float button_width = ImGui::GetContentRegionAvail().x;
    if (ImGui::Button("Render##render_button", ImVec2(button_width, 40))) {
        int width  = (int)size.x;
        int height = (int)size.y;

        if (width > 0 || height > 0) {
            start_render(width, height);
        }
    }


    ImGui::Spacing();
    ImGui::ProgressBar(m_render_progress.load(), ImVec2(-1, 0));

    ImGui::Spacing();
    if (eta > 0.5f) {
        ImGui::Text("ETA: %.1f s", eta);
    } else if (eta <= 0.5f && !m_framebuffer.empty()) {
        ImGui::Text("Completed");
    } else {
        ImGui::Text("Render not started");
    }

    ImGui::EndChild();

    ImGui::EndChild();

    // ImGui::ShowDemoWindow();
    ImGui::End();
}

void AppLayer::start_render(int width, int height) {

    if (m_render_thread.joinable()) {
        m_cancel_rendering = true;
        m_render_thread.join();
    }

    m_cancel_rendering = false;
    m_is_rendering = true;

    m_render_thread = std::thread([this, width, height]() {
        std::vector<uint32_t> local_buffer;

        m_camera.set_resolution(width, height);
        m_camera.set_progress_callback([this](float p) {m_render_progress = p; });

        m_render_start = std::chrono::steady_clock::now();
        m_render_progress = 0.0f;

        m_camera.render_to_buffer(m_world, local_buffer, m_cancel_rendering);


        if (!m_cancel_rendering) {
            std::lock_guard<std::mutex> lock(m_framebuffer_mutex);
            m_framebuffer = std::move(local_buffer);
        }

        m_is_rendering = false;
    });

}

void AppLayer::init_scene() {

    m_world = hittable_list();

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    m_world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_real();
            point3 center(a + 0.9*random_real(), 0.2, b + 0.9*random_real());
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    m_world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_real(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    m_world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    m_world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    m_world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    m_world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    m_world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    m_camera = Camera();

    m_camera.samples_per_pixel   = 10;
    m_camera.max_depth           = 5;

    m_camera.vfov        = 20;
    m_camera.lookfrom    = point3(13,2,3);
    m_camera.lookat      = point3(0,0,0);
    m_camera.vup         = vec3(0,1,0);

    m_camera.defocus_angle   = 0.6;
    m_camera.focus_dist      = 10.0;
}
