#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Window.hpp>

struct PerspectiveCamera;
class Keyboard;

void free_camera_controller_input(const Keyboard& keyboard, PerspectiveCamera& camera, sf::Time dt,
                                  const sf::Window& window, bool is_rotation_locked);