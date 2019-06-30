#ifndef GAME_STATE_HIGHSCORE_HPP_INCLUDEGUARD
#define GAME_STATE_HIGHSCORE_HPP_INCLUDEGUARD

#include "JamTemplate/GameState.hpp"

namespace JamTemplate {
class SmartText;
class SmartShape;
class SmartSprite;
class Animation;
}

class Hud;

class StateHighscore : public JamTemplate::GameState {
public:
    StateHighscore(std::shared_ptr<Hud> hud);

private:
    std::vector<std::shared_ptr<JamTemplate::Animation>> m_backgroundAnims;
    std::shared_ptr<JamTemplate::SmartShape> m_backgroundYellow;
    std::shared_ptr<JamTemplate::SmartShape> m_backgroundRed;

    std::shared_ptr<JamTemplate::Animation> m_playerWin;
    std::shared_ptr<JamTemplate::Animation> m_playerLoose;

    std::shared_ptr<JamTemplate::SmartShape> m_overlay;

    std::shared_ptr<Hud> m_hud;

    bool m_starting { false };

    void doInternalUpdate(float const /*elapsed*/) override;
    void doCreate() override;
    void doInternalDraw() const override;
};

#endif
