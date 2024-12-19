#include "CameraController.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Graphics/Camera.h"
#include "Util/Keyboard.h"

void free_camera_controller_input(const Keyboard& keyboard, PerspectiveCamera& camera, sf::Time dt,
                                  const sf::Window& window, bool is_rotation_locked)
{
    if (!window.hasFocus())
    {
        return;
    }

    // Keyboard Input
    glm::vec3 move{0.0f};
    if (keyboard.is_key_down(sf::Keyboard::W))
    {
        move += forward_vector(camera.transform.rotation);
    }
    else if (keyboard.is_key_down(sf::Keyboard::S))
    {
        move += backward_vector(camera.transform.rotation);
    }

    if (keyboard.is_key_down(sf::Keyboard::A))
    {
        move += left_vector(camera.transform.rotation);
    }
    else if (keyboard.is_key_down(sf::Keyboard::D))
    {
        move += right_vector(camera.transform.rotation);
    }

    if (keyboard.is_key_down(sf::Keyboard::LShift))
    {
        move *= 10.0f;
    }
    camera.transform.position += move * dt.asSeconds();
    camera.update();

    if (!is_rotation_locked)
    {
        static auto last_mouse = sf::Mouse::getPosition(window);
        auto change = sf::Mouse::getPosition(window) - last_mouse;
        auto& r = camera.transform.rotation;

        r.x -= static_cast<float>(change.y * 0.35);
        r.y += static_cast<float>(change.x * 0.35);

        sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2}, window);
        last_mouse = sf::Mouse::getPosition(window);

        r.x = glm::clamp(r.x, -89.9f, 89.9f);
        if (r.y >= 360.0f)
        {
            r.y = 0.0f;
        }
        else if (r.y < 0.0f)
        {
            r.y = 359.9f;
        }
    }
}