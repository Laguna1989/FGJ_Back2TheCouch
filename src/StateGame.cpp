#include "StateGame.hpp"

#include "GameProperties.hpp"
#include "Hud.hpp"
#include "JamTemplate/Collision.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartSprite.hpp"
#include "JamTemplate/TweenAlpha.hpp"

void StateGame::doInternalUpdate(float const elapsed)
{
    m_overlay->update(elapsed);
    m_world->Step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());

    for (auto s : *m_shots) {
        auto shot = s.lock();
        if (shot == nullptr) {
            continue;
        }
        if (!shot->isAlive() || !shot->m_movementStarted || shot->m_collided) {
            continue;
        }
        for (auto t : *m_level->getTiles()) {
            auto tile = t.lock();
            if (tile == nullptr) {
                continue;
            }

            if (JamTemplate::Collision::BoundingBoxTest(tile->getSprite(), shot->getSprite())) {
                //shot->kill();
                shot->Collide();
                break;
            }
        }
    }

    for (auto p : *m_players) {
        auto player = p.lock();
        if (player == nullptr) {
            continue;
        }

        if (JamTemplate::Collision::BoundingBoxTest(player->getSprite(), m_lava->getSprite())) {
            respawn(player);
        }
    }
}

void StateGame::doInternalDraw() const
{
    m_level->m_backgroundImage->draw(getGame()->getRenderTarget());
    drawObjects();
    m_overlay->draw(getGame()->getRenderTarget());
    m_lava->draw();
    m_hud->draw();
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
    m_players = std::make_shared<JamTemplate::ObjectGroup<Player>>();
    add(m_players);
    AddPlayer(0);
    AddPlayer(1);
    m_shots = std::make_shared<JamTemplate::ObjectGroup<Shot>>();
    m_lava = std::make_shared<Lava>(*this, 2143);
    add(m_lava);
}

void StateGame::AddPlayer(int id)
{
    b2BodyDef playerBodyDef;
    playerBodyDef.fixedRotation = true;
    playerBodyDef.allowSleep = false;
    playerBodyDef.type = b2_dynamicBody;

    auto player = std::make_shared<Player>(*this, &playerBodyDef, id);
    player->setPosition(m_level->getSpawnPosition(id));
    add(player);
    m_players->emplace_back(player);
}

void StateGame::respawn(std::shared_ptr<Player> player)
{
    // TODO: modify scores to reflect player's failure
    // TODO: wait for a while
    auto pos = m_level->getSpawnPositionAbove(m_lava->getHeight());
    player->setPosition(pos);
}
