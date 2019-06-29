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
    JamTemplate::Animation::Sptr getSprite() { return m_sprite; };

private:
    StateGame& m_gameState;
    int m_id { 0 };
    PlayerInput m_input;

    JamTemplate::Animation::Sptr m_sprite;

    JamTemplate::SmartShape::Sptr m_closeCombatAttackArea;
    bool m_drawCloseCombatAttackArea { false };

    bool m_facingRight { true };

    float m_shotTimer { 0.0f };

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();

    void updateMovement(float const elapsed);
    void updateAnimation();

    void updateShot(float const elapsed);
    void SpawnShot();
};

#endif
