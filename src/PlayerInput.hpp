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
    bool hasJustPressedJump()
    {
        return im::justPressed(m_jumpKey);
    }
    bool hasJustPressedShot()
    {
        return im::justPressed(m_shotKey);
    }

private:
    int m_id { 0 };

    void setKeys()
    {
        if (m_id == 0) {
            m_leftKey = sf::Keyboard::Key::Left;
            m_rightKey = sf::Keyboard::Key::Right;
            m_jumpKey = sf::Keyboard::Key::Up;
            m_shotKey = sf::Keyboard::Key::RShift;
        } else if (m_id == 1) {
            m_leftKey = sf::Keyboard::Key::A;
            m_rightKey = sf::Keyboard::Key::D;
            m_jumpKey = sf::Keyboard::Key::W;
            m_shotKey = sf::Keyboard::Key::LShift;
        }
    }

    sf::Keyboard::Key m_rightKey;
    sf::Keyboard::Key m_leftKey;
    sf::Keyboard::Key m_jumpKey;
    sf::Keyboard::Key m_shotKey;
};

#endif
