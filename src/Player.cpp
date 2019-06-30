
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
    dynamicBox.SetAsBox(static_cast<float32>(GP::SpriteSize()) / 2.0f - 2.5, static_cast<float32>(GP::SpriteSize()) / 2.0f);

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

    if (m_id == 0) {
        // Player 1 sounds
        m_soundBufferHop.loadFromFile("assets/hop.wav");
        m_soundBufferShot.loadFromFile("assets/shot.wav");
    } else {
        // Player 2 sounds
        m_soundBufferHop.loadFromFile("assets/hop2.wav");
        m_soundBufferShot.loadFromFile("assets/shot2.wav");
    }
    m_hopSound.setBuffer(m_soundBufferHop);
    m_shotSound.setBuffer(m_soundBufferShot);
    m_shotSound.setVolume(50);
}

void Player::doUpdate(float const elapsed)
{
    if (m_active) {
        updateShot(elapsed);
        updateMovement(elapsed);

        updateAnimation();
        m_sprite->update(elapsed);
        m_sprite->setPosition(getPosition());

        m_closeCombatAttackArea->setPosition(getPosition());
        m_closeCombatAttackArea->update(elapsed);

        if (getPosition().x <= 0) {
            setPosition(sf::Vector2f(0, getPosition().y));
        }
        if (getPosition().x >= getGame()->getRenderTarget()->getSize().x - GP::SpriteSize()) {
            setPosition(sf::Vector2f(getGame()->getRenderTarget()->getSize().x - GP::SpriteSize(), getPosition().y));
        }
    }
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
        m_facingRight = false;
    }
    if (m_input.isRightPressed()) {
        getB2Body()->ApplyForceToCenter(b2Vec2 { GP::PlayerMovementAcceleration(), 0 }, true);
        m_facingRight = true;
    }
    if (m_input.hasJustPressedJump() && fabs(getB2Body()->GetLinearVelocity().y) < GP::PlayerVerticalSpeedJumpThreshold()) {
        m_hopSound.play();
        getB2Body()->ApplyLinearImpulseToCenter(b2Vec2 { 0, -GP::PlayerJumpImpulse() }, true);
    }
    sf::Vector2f vel = JamTemplate::C::vec(getB2Body()->GetLinearVelocity());

    float actualVelX = MH::clamp(vel.x, -GP::PlayerMaxSpeedHorizontal(), GP::PlayerMaxSpeedHorizontal());
    // Jumps can be as fast as the impulse allows (hence -inf).
    float actualVelY = MH::clamp(vel.y, -INFINITY, GP::PlayerMaxSpeedVertical());

    getB2Body()->SetLinearVelocity(b2Vec2 { actualVelX, actualVelY });

    float vl = JamTemplate::MathHelper::length(vel);
    /*
    if (std::fabs(vl) > 0.005f) // do exponential damping
    {
        float d = (1.0f - GP::SpriteLinearDamping() * frameRateCorrection);
        getB2Body()->SetLinearVelocity(b2Vec2 { vel.x * d, vel.y * d });
    }*/
}

void Player::SpawnShot()
{

    m_shotSound.play();
    m_shotTimer = GP::ShotTimer();
    sf::Vector2f ofs { 0, -4 };
    sf::Vector2f vel { GP::ShotVelocityStart(), JamTemplate::Random::getFloat(-10, 5) };
    b2Vec2 recoilImpulse;
    if (m_facingRight) {
        ofs.x += GP::SpriteSize();
        recoilImpulse = b2Vec2 { -GP::ShotRecoilHorizontal(), -GP::ShotRecoilVertical() };
    } else {
        ofs.x -= GP::SpriteSize();
        vel.x *= -1;
        recoilImpulse = b2Vec2 { GP::ShotRecoilHorizontal(), -GP::ShotRecoilVertical() };
    }
    getB2Body()->ApplyLinearImpulseToCenter(recoilImpulse, true);
    std::shared_ptr<Shot> shot = std::make_shared<Shot>(m_gameState);
    shot->m_firedBy = m_id;
    shot->setPosition(getPosition() + ofs);
    shot->setVelocity(vel);
    shot->setAcceleration(sf::Vector2f { vel.x / GP::ShotVelocityStart() * 300, 0 });
    auto svm = GP::ShotVelocityMax();
    shot->setBoundsVelocity(sf::FloatRect(-svm, -svm, 2 * svm, 2 * svm));

    m_gameState.SpawnShot(shot);
}

void Player::getHitByShot(std::shared_ptr<Shot> shot)
{
    m_sprite->flash(0.25);
    m_sprite->shake(0.2, 1);
    b2Vec2 knockbackImpulse;
    if (shot->getVelocity().x < 0.0f) {
        knockbackImpulse = b2Vec2 { -GP::ShotKnockbackHorizontal(), -GP::ShotKnockbackVertical() };
    } else {
        knockbackImpulse = b2Vec2 { GP::ShotKnockbackHorizontal(), -GP::ShotKnockbackVertical() };
    }
    getB2Body()->ApplyLinearImpulseToCenter(knockbackImpulse, true);
}
