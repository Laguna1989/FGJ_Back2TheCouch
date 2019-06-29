#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include <Box2D/Box2D.h>
#include <iostream>
#include <memory>
#include <vector>

#include "JamTemplate/GameState.hpp"
#include "Level.hpp"
#include "Player.hpp"

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

protected:
    std::shared_ptr<Hud> m_hud;

private:
    std::shared_ptr<b2World> m_world { nullptr };
    std::shared_ptr<JamTemplate::SmartShape> m_overlay;
    std::shared_ptr<Player> m_player;
    std::shared_ptr<Level> m_level;

    void doCreate() override;

    virtual void doCreateInternal();

    virtual void doInternalUpdate(float const elapsed) override;
    virtual void doInternalDraw() const override;
};

#endif
