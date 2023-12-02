#include "GUI.h"

#include <imgui.h>
#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui_impl_opengl3.h>

#include "Util.h"

namespace
{
    void base_light_widgets(LightBase& light)
    {
        ImGui::SliderFloat3("Colour", &light.colour[0], 0.0, 1.0);
        ImGui::SliderFloat("Ambient Intensity", &light.ambient_intensity, 0.0, 1.0);
        ImGui::SliderFloat("Diffuse Intensity", &light.diffuse_intensity, 0.0, 1.0);
        ImGui::SliderFloat("Specular Intensity", &light.specular_intensity, 0.0, 1.0);
    }

    void attenuation_widgets(Attenuation& attenuation)
    {
        ImGui::SliderFloat("Attenuation Constant", &attenuation.constant, 0.0, 1.0f);
        ImGui::SliderFloat("Attenuation Linear", &attenuation.linear, 0.14f, 0.0014f, "%.6f");
        ImGui::SliderFloat("Attenuation Quadratic", &attenuation.exponant, 0.000007f, 0.03f,
                           "%.6f");
    }
}

namespace GUI
{
    void init(sf::Window* window)
    {
        ImGui::SFML::Init(*window, cast_vector<float>(window->getSize()));
        ImGui_ImplOpenGL3_Init();
    }

    void begin_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::SFML::Shutdown();
    }

    void render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void event(const sf::Window& window, sf::Event& e)
    {
        ImGui::SFML::ProcessEvent(window, e);
    }

    void debug_window(const glm::vec3& camera_position, const glm::vec3& camera_rotation,
                      Settings& settings)
    {
        auto r = camera_rotation;
        auto p = camera_position;

        // clang-format off
        if (ImGui::Begin("Debug Window"))
        {
            ImGui::Text("Position: (%f, %f, %f)", p.x, p.y, p.z);
            ImGui::Text("Rotation: (%f, %f, %f)", r.x, r.y, r.z);

            ImGui::SliderFloat("Material Shine", &settings.material_shine, 1.0f, 64.0f);

            ImGui::Separator();
            ImGui::Checkbox("Grass ground?", &settings.grass);

            ImGui::Separator();

            ImGui::PushID("DirLight");
            ImGui::Text("Directional light");
            if (ImGui::SliderFloat3("Direction", &settings.dir_light.direction[0], -1.0, 1.0))
            {
                settings.dir_light.direction = glm::normalize(settings.dir_light.direction);
            }
            base_light_widgets(settings.dir_light);
            ImGui::PopID();

            ImGui::Separator();

            ImGui::PushID("PointLight");
            ImGui::Text("Point light");
            base_light_widgets(settings.point_light);
            attenuation_widgets(settings.point_light.att);
            ImGui::PopID();

            ImGui::Separator();

            ImGui::PushID("SpotLight");
            ImGui::Text("Spot light");
            ImGui::SliderFloat("Cutoff", &settings.spot_light.cutoff, 0.0, 90.0f);
            base_light_widgets(settings.spot_light);
            attenuation_widgets(settings.spot_light.att);
            ImGui::PopID();
        }
        // clang-format on

        ImGui::End();
    }

} // namespace GUI