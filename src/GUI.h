#pragma once

#include <SFML/Window/Window.hpp>

#include "Settings.h"


namespace GUI
{

    void init(sf::Window* window);

    void begin_frame();

    void shutdown();
    void render();

    void event(const sf::Window& window, sf::Event& e);


    void debug_window(const glm::vec3& camera_position,
                      const glm::vec3& camera_rotation, Settings& settings);

} // namespace GUI