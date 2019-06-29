
#include "Player.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/InputManager.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "Shot.hpp"
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

    std::string loaderName = "#r#assets/player.png#" + std::to_string(m_id);

    m_sprite->add(loaderName, "idle", sf::Vector2u { GP::SpriteSize(), GP::SpriteSize() }, JamTemplate::MathHelper::vectorBetween(0U, 0U), 0.15f);
    m_sprite->add(loaderName, "walk", sf::Vector2u { GP::SpriteSize(), GP::SpriteSize() }, JamTemplate::MathHelper::vectorBetween(0U, 3U), 0.1f);
    m_sprite->play("idle");

    m_closeCombatAttackArea
        = std::make_shared<JamTemplate::SmartShape>();
    m_closeCombatAttackArea->makeRect(sf::Vector2f { 24, 20 });
    m_closeCombatAttackArea->setOffset(sf::Vector2f { 8, 6 });
    m_closeCombatAttackArea->setOrigin(sf::Vector2f { 8, 10 });
}

void Player::doUpdate(float const elapsed)
{
    updateShot(elapsed);
    updateMovement(elapsed);

    updateAnimation();
    m_sprite->update(elapsed);
    m_sprite->setPosition(getPosition());

    m_closeCombatAttackArea->setPosition(getPosition());
    m_closeCombatAttackArea->update(elapsed);
}

void Player::updateShot(float const elapsed)
{
    m_shotTimer -= elapsed;
    if (m_shotTimer > 0) {
        return;
    }
    if (m_input.hasJustPressedShot()) {
        SpawnShot();
    }
}

void Player::updateAnimation()
{
    auto vel = JamTemplate::C::vec(getB2Body()->GetLinearVelocity());
    if (fabs(vel.x) > 10) {
        m_sprite->play("walk");
        m_facingRight = (vel.x > 0);
    } else {
        m_sprite->play("idle");
    }

    if (m_facingRight) {
        m_sprite->setScale(sf::Vector2f { 1, 1 });
        m_sprite->setOffset(sf::Vector2f { 0.0f, 0.0f });
    } else {
        m_sprite->setScale(sf::Vector2f { -1, 1 });
        m_sprite->setOffset(sf::Vector2f { static_cast<float>(GP::SpriteSize()), 0.0f });
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
    if (m_input.hasJustPressedJump() && fabs(getB2Body()->GetLinearVelocity().y) < GP::PlayerVerticalSpeedJumpThreshold()) {
        getB2Body()->ApplyLinearImpulseToCenter(b2Vec2 { 0, -GP::PlayerJumpImpulse() }, true);
    }
    sf::Vector2f vel = JamTemplate::C::vec(getB2Body()->GetLinearVelocity());

    float actualVelX = MH::clamp(vel.x, -GP::PlayerMaxSpeedHorizontal(), GP::PlayerMaxSpeedHorizontal());
    // Jumps can be as fast as the impulse allows (hence -inf).
    float actualVelY = MH::clamp(vel.y, -INFINITY, GP::PlayerMaxSpeedVertical());

    getB2Body()->SetLinearVelocity(b2Vec2 { actualVelX, actualVelY });

    float vl = JamTemplate::MathHelper::length(vel);
    /*
	// Debug print: print the clamped velocity vector
    if (vl > 0.001f) {
        std::cout << actualVelX << ", " << actualVelY << std::endl;
    }
	*/

    if (std::fabs(vl) > 0.005f) // do exponential damping
    {
        float d = 1.0f - GP::SpriteLinearDamping();
        getB2Body()->SetLinearVelocity(b2Vec2 { vel.x * d, vel.y * d });
    }
}

void Player::SpawnShot()
{
    m_shotTimer = GP::ShotTimer();
    sf::Vector2f ofs { 0, -4 };
    sf::Vector2f vel { GP::ShotVelocity(), JamTemplate::Random::getFloat(-10, 10) };
    if (m_facingRight) {
        ofs.x += GP::SpriteSize();
    } else {
        ofs.x -= GP::SpriteSize();
        vel.x *= -1;
    }
    std::shared_ptr<Shot> shot = std::make_shared<Shot>(m_gameState);
    shot->setPosition(getPosition() + ofs);
    shot->setVelocity(vel);

    m_gameState.SpawnShot(shot);
}
