#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "Couch.hpp"
#include "JamTemplate/GameState.hpp"
#include "JamTemplate/ObjectGroup.hpp"
#include "Lava.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Shot.hpp"
#include <Box2D/Box2D.h>
#include <iostream>
#include <memory>
#include <vector>

// fwd decls
namespace JamTemplate {
class SmartShape;
}

class Hud;

class StateGame : public JamTemplate::GameState {
public:
    StateGame()
        : m_world { std::make_shared<b2World>(b2Vec2 { 0, 0 }) } {};

    std::shared_ptr<b2World> getWorld()
    {
        return m_world;
    };

    void SpawnShot(std::shared_ptr<Shot> shot)
    {
        add(shot);
        m_shots->emplace_back(shot);
    }

    float getLavaHeight() { return m_lava->getHeight(); };

protected:
    std::shared_ptr<Hud> m_hud;

private:
    std::shared_ptr<b2World> m_world { nullptr };
    std::shared_ptr<Lava> m_lava;
    std::shared_ptr<Couch> m_couch;
    std::shared_ptr<JamTemplate::SmartShape> m_overlay;
    JamTemplate::ObjectGroup<Player>::Sptr m_players;
    JamTemplate::ObjectGroup<Shot>::Sptr m_shots;
    std::shared_ptr<Level> m_level;
    bool m_switching { false };
    bool m_returnToCouch { false };

    sf::SoundBuffer m_soundBufferLavaDrop {};
    sf::Sound m_lavaDropSound {};
    sf::SoundBuffer m_soundBufferEndGame {};
    sf::Sound m_endGameSound {};

    void doCreate() override;

    virtual void doCreateInternal();

    virtual void doInternalUpdate(float const elapsed) override;
    void CheckForTimeEnd();
    void EndGame();
    virtual void doInternalDraw() const override;

    void AddPlayer(int id);
    void respawn(std::shared_ptr<Player>);
    void score(int playerId, int score);

    void CollideShotsTiles(std::shared_ptr<Shot> shot);
    void CollideShotsPlayers(std::shared_ptr<Shot> shot);
    void CollidePlayersLava();
    void CollidePlayersCouch();
};

#endif
