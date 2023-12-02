#include "GUI.h"

#include <imgui.h>
#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui_impl_opengl3.h>

#include "Util.h"

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

} // namespace GUI