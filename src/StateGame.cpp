#include "StateGame.hpp"

#include "GameProperties.hpp"
#include "Hud.hpp"
#include "JamTemplate/Collision.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartSprite.hpp"
#include "JamTemplate/TweenAlpha.hpp"
#include "StateHighScore.hpp"

void StateGame::doInternalUpdate(float const elapsed)
{
    m_overlay->update(elapsed);
    m_world->Step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());

    if (!m_switching) {

        CheckForTimeEnd();

        for (auto s : *m_shots) {
            auto shot = s.lock();
            if (shot == nullptr) {
                continue;
            }
            if (shot->isShotInValid()) {
                continue;
            }
            CollideShotsTiles(shot);
            CollideShotsPlayers(shot);
        }

        CollidePlayersLava();
    }
    if (!m_returnToCouch && getAge() >= GP::ReturnToCouchTime()) {
        m_returnToCouch = true;
        m_hud->setReturnToCouchTextVisible(true);
        // TODO: Enable high scoring + game end on couch collision
    }
}

void StateGame::CheckForTimeEnd()
{
    if (GP::TotalGameTime() - getAge() < 0) {
        m_switching = true;
        auto tw = JamTemplate::TweenAlpha<JamTemplate::SmartShape>::create(m_overlay, 1.25f, sf::Uint8 { 0 }, sf::Uint8 { 255 });

        tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateHighscore>(m_hud)); });
        add(tw);
        for (auto p : *m_players) {
            p.lock()->Deactivate();
        }
    }
}

void StateGame::CollideShotsTiles(std::shared_ptr<Shot> shot)
{
    for (auto t : *m_level->getTiles()) {
        auto tile = t.lock();
        if (tile == nullptr) {
            continue;
        }

        if (JamTemplate::Collision::BoundingBoxTest(tile->getSprite(), shot->getSprite())) {
            shot->Collide();
            break;
        }
    }
}
void StateGame::CollideShotsPlayers(std::shared_ptr<Shot> shot)
{
    for (size_t i = 0; i != m_players->size(); ++i) {
        auto p = m_players->at(i);
        size_t iOther = (i + 1) % 2;
        auto player = p.lock();
        if (player == nullptr) {
            continue;
        }
        if (JamTemplate::Collision::BoundingBoxTest(shot->getSprite(), player->getSprite())) {
            shot->Collide();
            player->getHitByShot(shot);
            score(m_players->at(iOther).lock()->getId(), GP::PointsForHit());
        }
    }
}
void StateGame::CollidePlayersLava()
{
    for (size_t i = 0; i != m_players->size(); ++i) {
        auto p = m_players->at(i);
        size_t iOther = (i + 1) % 2;

        auto player = p.lock();
        if (JamTemplate::Collision::BoundingBoxTest(player->getSprite(), m_lava->getSprite())) {
            respawn(player);
            score(m_players->at(iOther).lock()->getId(), GP::PointsForOtherPlayerDyingInAFire());
            player->update(0.0f);
        }
    }
}

void StateGame::doInternalDraw() const
{
    m_level->m_backgroundImage->draw(getGame()->getRenderTarget());
    m_couch->draw();
    drawObjects();
    m_lava->draw();
    m_overlay->draw(getGame()->getRenderTarget());
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
    m_level->update(0.0f);
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
    b2BodyDef couchBodyDef;
    couchBodyDef.fixedRotation = true;
    couchBodyDef.allowSleep = false;
    couchBodyDef.type = b2_dynamicBody;
    m_couch = std::make_shared<Couch>(*this, &couchBodyDef);
    add(m_couch);
    add(m_lava);
}

void StateGame::AddPlayer(int id)
{
    b2BodyDef playerBodyDef;
    playerBodyDef.fixedRotation = true;
    playerBodyDef.allowSleep = false;
    playerBodyDef.type = b2_dynamicBody;

    auto player = std::make_shared<Player>(*this, &playerBodyDef, id);
    player->setPosition(GP::CouchSpawnPosition() + sf::Vector2f { id * 36.0f, -24.0f });
    player->setFacingRight(id == 0);
    add(player);
    m_players->emplace_back(player);
}

void StateGame::respawn(std::shared_ptr<Player> player)
{
    auto pos = m_level->getSpawnPositionAbove(m_lava->getHeight());
    player->setPosition(pos);
}

void StateGame::score(int playerId, int score)
{
    if (playerId == 0) {
        m_hud->AddScoreP1(score);
    } else {
        m_hud->AddScoreP2(score);
    }
}
