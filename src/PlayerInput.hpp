#ifndef PLAYERINPUT_HPP_GUARD
#define PLAYERINPUT_HPP_GUARD

#include "JamTemplate/InputManager.hpp"
#include <SFML/Window.hpp>

class PlayerInput {
public:
    using im = JamTemplate::InputManager;
    PlayerInput(int id)
        : m_id { id }
    {
        setKeys();
    }

    bool isLeftPressed()
    {
        return im::pressed(m_leftKey);
    }
    bool isRightPressed()
    {
        return im::pressed(m_rightKey);
    }

private:
    int m_id { 0 };

    void setKeys()
    {
        if (m_id == 0) {
            m_leftKey = sf::Keyboard::Key::Left;
            m_rightKey = sf::Keyboard::Key::Right;
        } else if (m_id == 1) {
            m_leftKey = sf::Keyboard::Key::A;
            m_rightKey = sf::Keyboard::Key::D;
        }
    }

    sf::Keyboard::Key m_rightKey;
    sf::Keyboard::Key m_leftKey;
};

#endif
