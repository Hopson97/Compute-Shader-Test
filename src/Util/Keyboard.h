
#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>

class Keyboard final
{
  public:
    Keyboard();
    void update(const sf::Event& e);
    bool is_key_down(sf::Keyboard::Key key) const;

    void reset();

  private:

    std::array<bool, sf::Keyboard::KeyCount> m_keys;
};