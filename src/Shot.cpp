
#include "Shot.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "StateGame.hpp"
#include <iostream>

Shot::Shot(StateGame& sg)
    : m_stateGame { sg }
{
}

void Shot::doCreate()
{
    m_sprite = std::make_shared<JamTemplate::Animation>();
    m_sprite->add("assets/shot.png", "spawn", sf::Vector2u { 12, 12 }, JamTemplate::MathHelper::vectorBetween(1U, 1U), 0.1f);
    m_sprite->add("assets/shot.png", "idle", sf::Vector2u { 12, 12 }, JamTemplate::MathHelper::vectorBetween(3U, 7U), 0.05f);
    m_sprite->add("assets/shot.png", "clash", sf::Vector2u { 12, 12 }, JamTemplate::MathHelper::vectorBetween(8U, 11U), 0.075f);
    m_sprite->play("spawn");

    if (getVelocity().x < 0) {
        m_sprite->setScale(sf::Vector2f { -1, 1 });
        m_sprite->setOffset(sf::Vector2f { static_cast<float>(GP::SpriteSize()), 0.0f });
    }
}

void Shot::doUpdate(float const elapsed)
{
    if (static_cast<float>(getAge() - elapsed) < 0.1 && getAge() >= 0.1) {
        Start2Move();
    }

    if (m_movementStarted && !m_collided) {
        updateTransform(elapsed);
    }

    m_sprite->setPosition(getPosition());
    m_sprite->update(elapsed);

    if (getAge() > 5) {
        kill();
    }
}

void Shot::doDraw() const
{
    m_sprite->draw(getGame()->getRenderTarget());
}

JamTemplate::Animation::Sptr Shot::getSprite() { return m_sprite; };
