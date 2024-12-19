#pragma once

#include <SFML/Window/Window.hpp>
#include <glm/glm.hpp>

namespace GUI
{

    bool init(sf::Window* window);

    void begin_frame();
    void update();

    void shutdown();
    void render();

    void event(const sf::Window& window, sf::Event& e);

    void debug_window(const glm::vec3& camera_position, const glm::vec3& camera_rotation);

} // namespace GUI