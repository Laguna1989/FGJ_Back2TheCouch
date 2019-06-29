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
    m_world->Step(elapsed, GP::velocityIterations(), GP::positionIterations());
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    drawObjects();
    m_overlay->draw(getGame()->getRenderTarget());
}

void StateGame::doCreate()
{

    m_world->SetGravity(b2Vec2 { 0, 1000 });

    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    m_hud = std::make_shared<Hud>();

    using JamTemplate::SmartShape;
    using JamTemplate::TweenAlpha;

    m_background = std::make_shared<SmartShape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->update(0.0f);

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

    b2BodyDef playerBodyDef;
    playerBodyDef.fixedRotation = true;
    playerBodyDef.allowSleep = false;
    playerBodyDef.type = b2_dynamicBody;
    m_player = std::make_shared<Player>(*this, &playerBodyDef);
    add(m_player);
}
