#include "StateGame.hpp"

#include "GameProperties.hpp"
#include "Hud.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartSprite.hpp"
#include "JamTemplate/TweenAlpha.hpp"

void StateGame::doInternalUpdate(float const elapsed)
{
    m_overlay->update(elapsed);
    m_world->Step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
}

void StateGame::doInternalDraw() const
{
    drawObjects();
    m_overlay->draw(getGame()->getRenderTarget());
}

void StateGame::doCreate()
{

    m_world->SetGravity(b2Vec2 { 0, GP::PhysicsGravity() });

    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    m_hud = std::make_shared<Hud>();

    using JamTemplate::SmartShape;
    using JamTemplate::TweenAlpha;

    m_overlay = std::make_shared<SmartShape>();
    m_overlay->makeRect(sf::Vector2f { w, h });
    m_overlay->setColor(sf::Color { 0, 0, 0 });
    m_overlay->update(0);
    auto tw = TweenAlpha<SmartShape>::create(m_overlay, 0.5f, sf::Uint8 { 255 }, sf::Uint8 { 0 });
    tw->setSkipFrames();
    add(tw);

    doCreateInternal();
    add(m_hud);
}

void StateGame::doCreateInternal()
{

    m_level = std::make_shared<Level>(*this);
    m_level->LoadLevel("assets/level1.png");
    add(m_level);

    b2BodyDef playerBodyDef;
    playerBodyDef.fixedRotation = true;
    playerBodyDef.allowSleep = false;
    playerBodyDef.type = b2_dynamicBody;
    m_player = std::make_shared<Player>(*this, &playerBodyDef);
    add(m_player);
}
