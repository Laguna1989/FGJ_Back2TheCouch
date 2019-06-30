#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "JamTemplate/GameObject.hpp"
#include "JamTemplate/SmartText.hpp"
#include <string>

class Hud : public JamTemplate::GameObject {
public:
    Hud();

    void AddScoreP1(int i = 1);
    void AddScoreP2(int i = 1);
    void setReturnToCouchTextVisible(bool visible) { m_showReturnToCouchText = visible; };

    int getScoreP1() const { return m_scoreP1; };
    int getScoreP2() const { return m_scoreP2; };

    void DisableTimer() { m_drawTimer = false; };

private:
    int m_scoreP1 { 0 };
    int m_scoreP2 { 0 };
    bool m_showReturnToCouchText { false };

    bool m_drawTimer { true };

    JamTemplate::SmartText::Sptr m_scoreP1Text;
    JamTemplate::SmartText::Sptr m_scoreP2Text;
    JamTemplate::SmartText::Sptr m_timerText;
    JamTemplate::SmartText::Sptr m_returnToCouchText;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;

    void doCreate() override;
};

#endif
