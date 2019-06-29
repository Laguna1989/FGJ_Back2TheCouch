
#include "Player.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/InputManager.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "StateGame.hpp"
#include <iostream>

Player::Player(StateGame& sg, const b2BodyDef* def)
    : m_gameState(sg)
    , JamTemplate::Box2DObject { sg.getWorld(), def }
{
    setB2Obj();
}

void Player::setB2Obj()
{
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(GP::SpriteSize() / 2, GP::SpriteSize() / 2);

    b2FixtureDef fixtureDef;

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = GP::PlayerDensity();
    fixtureDef.friction = GP::PlayerFriction();

    getB2Body()->CreateFixture(&fixtureDef);
    getB2Body()->SetLinearDamping(GP::SpriteLinearDamping());
}

void Player::doCreate()
{
    setPosition(sf::Vector2f { 20, 14 });
    m_sprite = std::make_shared<JamTemplate::Animation>();
    m_sprite->add("assets/coin.png", "idle", sf::Vector2u { GP::SpriteSize(), GP::SpriteSize() }, JamTemplate::MathHelper::vectorBetween(0U, 11U), 0.15f);
    m_sprite->play("idle");

    m_closeCombatAttackArea = std::make_shared<JamTemplate::SmartShape>();
    m_closeCombatAttackArea->makeRect(sf::Vector2f { 24, 20 });
    m_closeCombatAttackArea->setOffset(sf::Vector2f { 8, 6 });
    m_closeCombatAttackArea->setOrigin(sf::Vector2f { 8, 10 });
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);
    m_sprite->update(elapsed);
    m_sprite->setPosition(getPosition());

    m_closeCombatAttackArea->setPosition(getPosition());
    m_closeCombatAttackArea->update(elapsed);
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
    using im = JamTemplate::InputManager;
    using k = sf::Keyboard::Key;
    if (im::pressed(k::A) || im::pressed(k::Left)) {
        getB2Body()->ApplyForceToCenter(b2Vec2 { -GP::PlayerMovementAcceleration(), 0 }, true);
    }
    if (im::pressed(k::D) || im::pressed(k::Right)) {
        getB2Body()->ApplyForceToCenter(b2Vec2 { GP::PlayerMovementAcceleration(), 0 }, true);
    }
    
}
