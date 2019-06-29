#ifndef PLAYER_HPP_INCLUDEGUARD
#define PLAYER_HPP_INCLUDEGUARD

#include "JamTemplate/Animation.hpp"
#include "JamTemplate/Box2DObject.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "PlayerInput.hpp"
#include <Box2D/Box2D.h>

// forward declaration
class StateGame;

class Player : public JamTemplate::Box2DObject {
public:
    Player(StateGame& sg, const b2BodyDef* def, int id);

    void setB2Obj();

private:
    StateGame& m_gameState;
    int m_id { 0 };
    PlayerInput m_input;

    JamTemplate::Animation::Sptr m_sprite;

    JamTemplate::SmartShape::Sptr m_closeCombatAttackArea;
    bool m_drawCloseCombatAttackArea { false };

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();

    void updateMovement(float const elapsed);
};

#endif
