#include "Keyboard.h"

Keyboard::Keyboard()
{
    reset();
}

void Keyboard::update(const sf::Event& e)
{
    switch (e.type)
    {
        case sf::Event::KeyReleased:
            if (e.key.code == -1)
                return;
            m_keys[e.key.code] = false;
            break;

        case sf::Event::KeyPressed:
            if (e.key.code == -1)
                return;
            m_keys[e.key.code] = true;
            break;

        default:
            break;
    }
}

bool Keyboard::is_key_down(sf::Keyboard::Key key) const
{
    return m_keys[key];
}

void Keyboard::reset()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}