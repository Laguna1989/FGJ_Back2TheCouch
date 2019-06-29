
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

void Player::doUpdate(float const elapsed)
{
    //std::cout << "I am alive" << std::endl;
    m_sprite->setPosition(getPosition());
    m_sprite->update(elapsed);
}
void Player::doDraw() const
{
    m_sprite->draw(getGame()->getRenderTarget());
}

void Player::doCreate()
{
    m_sprite = std::make_shared<JamTemplate::Animation>();
    m_sprite->add("assets/coin.png", "idle", sf::Vector2u { 16, 16 }, JamTemplate::MathHelper::vectorBetween(0U, 11U), 0.15f);
    m_sprite->play("idle");
}
