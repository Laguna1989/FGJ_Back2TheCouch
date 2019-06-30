
#include "Couch.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/InputManager.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "Shot.hpp"
#include "StateGame.hpp"
#include <iostream>

Couch::Couch(StateGame& sg, const b2BodyDef* def)
    : m_gameState(sg)
{
}

void Couch::doCreate()
{
    m_sprite = std::make_shared<JamTemplate::SmartSprite>();
    m_sprite->loadSprite("assets/couch.png");
    // m_sprite->setOrigin(sf::Vector2f { static_cast<float>(GP::SpriteSize()), GP::SpriteSize() / 2.0f });
    setPosition(sf::Vector2f { 72.0f, 168.0f });
}

void Couch::doUpdate(float const elapsed)
{
    updateMovement(elapsed);
    m_sprite->setPosition(getPosition());
    //  m_sprite->setRotation(getRotation());
    m_sprite->update(elapsed);
}

void Couch::doDraw() const
{
    m_sprite->draw(getGame()->getRenderTarget());
}

void Couch::updateMovement(float const elapsed)
{
    float referenceCouchOffset = m_sprite->getLocalBounds().height;
    if (m_sprite->getPosition().y + referenceCouchOffset > m_gameState.getLavaHeight()) {
        float y = m_gameState.getLavaHeight() - referenceCouchOffset;
        float wobble = (1 + sin(getAge())) * GP::SpriteSize() / 2.0f;
        setPosition(sf::Vector2f { getPosition().x, m_gameState.getLavaHeight() - referenceCouchOffset + wobble });
    }
}
