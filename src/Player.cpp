
#include "Player.hpp"
#include "JamTemplate/Game.hpp"
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
    dynamicBox.SetAsBox(8, 8);

    b2FixtureDef fixtureDef;

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.09f;

    getB2Body()->CreateFixture(&fixtureDef);
    getB2Body()->SetLinearDamping(0.1f);
}

void Player::doCreate()
{
    m_sprite = std::make_shared<JamTemplate::Animation>();
    m_sprite->add("assets/coin.png", "idle", sf::Vector2u { 16, 16 }, JamTemplate::MathHelper::vectorBetween(0U, 11U), 0.15f);
    m_sprite->play("idle");

    m_collider = std::make_shared<JamTemplate::SmartShape>();
    m_collider->makeRect(sf::Vector2f { 24, 20 });
    m_collider->setOffset(sf::Vector2f { 8, 6 });
    m_collider->setOrigin(sf::Vector2f { 8, 10 });
}

void Player::doUpdate(float const elapsed)
{
    //std::cout << "I am alive" << std::endl;
    m_sprite->setPosition(getPosition());
    m_sprite->update(elapsed);

    m_collider->setPosition(getPosition());
    m_collider->update(elapsed);
}
void Player::doDraw() const
{
    m_sprite->draw(getGame()->getRenderTarget());

    if (m_drawCollider) {
        m_collider->draw(getGame()->getRenderTarget());
    }
}
