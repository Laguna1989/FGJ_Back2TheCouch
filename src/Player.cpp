
#include "Player.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/InputManager.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "StateGame.hpp"
#include <iostream>

Player::Player(StateGame& sg, const b2BodyDef* def, int id)
    : m_gameState(sg)
    , JamTemplate::Box2DObject { sg.getWorld(), def }
    , m_id { id }
    , m_input { id }
{
    setB2Obj();
}

void Player::setB2Obj()
{
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(static_cast<float32>(GP::SpriteSize()) / 2.0f, static_cast<float32>(GP::SpriteSize()) / 2.0f);

    b2FixtureDef fixtureDef;

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = GP::PlayerDensity();
    fixtureDef.friction = GP::PlayerFriction();

    getB2Body()->CreateFixture(&fixtureDef);
    getB2Body()->SetLinearDamping(GP::SpriteLinearDamping());
}

void Player::doCreate()
{
    m_sprite = std::make_shared<JamTemplate::Animation>();
    m_sprite->add("assets/player.png", "idle", sf::Vector2u { GP::SpriteSize(), GP::SpriteSize() }, JamTemplate::MathHelper::vectorBetween(0U, 1U), 0.15f);
    m_sprite->add("assets/player.png", "walk", sf::Vector2u { GP::SpriteSize(), GP::SpriteSize() }, JamTemplate::MathHelper::vectorBetween(0U, 6U), 0.15f);
    m_sprite->play("idle");
    //m_sprite->setColor(sf::Color { 0, 200, 140 });

    m_closeCombatAttackArea
        = std::make_shared<JamTemplate::SmartShape>();
    m_closeCombatAttackArea->makeRect(sf::Vector2f { 24, 20 });
    m_closeCombatAttackArea->setOffset(sf::Vector2f { 8, 6 });
    m_closeCombatAttackArea->setOrigin(sf::Vector2f { 8, 10 });
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);

    updateAnimation();
    m_sprite->update(elapsed);
    m_sprite->setPosition(getPosition());

    m_closeCombatAttackArea->setPosition(getPosition());
    m_closeCombatAttackArea->update(elapsed);
}
void Player::updateAnimation()
{
    auto vel = JamTemplate::C::vec(getB2Body()->GetLinearVelocity());
    if (fabs(vel.x) > 10) {
        m_sprite->play("walk");
    } else {
        m_sprite->play("idle");
    }
}

void Player::doDraw() const
{
    m_sprite->draw(getGame()->getRenderTarget());

    if (m_drawCloseCombatAttackArea) {
        m_closeCombatAttackArea->draw(getGame()->getRenderTarget());
    }
}

void Player::updateMovement(float const elapsed)
{
    using MH = JamTemplate::MathHelper;
    if (m_input.isLeftPressed()) {
        getB2Body()->ApplyForceToCenter(b2Vec2 { -GP::PlayerMovementAcceleration(), 0 }, true);
    }
    if (m_input.isRightPressed()) {
        getB2Body()->ApplyForceToCenter(b2Vec2 { GP::PlayerMovementAcceleration(), 0 }, true);
    }

    sf::Vector2f vel = JamTemplate::C::vec(getB2Body()->GetLinearVelocity());

    float actualVelX = MH::clamp(vel.x, -GP::PlayerMaxSpeedHorizontal(), GP::PlayerMaxSpeedHorizontal());
    float actualVelY = MH::clamp(vel.y, -GP::PlayerMaxSpeedVertical(), GP::PlayerMaxSpeedVertical());

    getB2Body()->SetLinearVelocity(b2Vec2 { actualVelX, actualVelY });

    float vl = JamTemplate::MathHelper::length(vel);

    if (std::fabs(vl) > 0.005f) // do exponential damping
    {
        float d = 1.0f - GP::SpriteLinearDamping();
        getB2Body()->SetLinearVelocity(b2Vec2 { vel.x * d, vel.y * d });
    }
}
