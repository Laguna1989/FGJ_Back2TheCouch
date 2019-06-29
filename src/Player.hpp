#ifndef PLAYER_HPP_INCLUDEGUARD
#define PLAYER_HPP_INCLUDEGUARD

#include "JamTemplate/Animation.hpp"
#include "JamTemplate/Box2DObject.hpp"
#include "JamTemplate/SmartShape.hpp"
#include <Box2D/Box2D.h>

// forward declaration
class StateGame;

class Player : public JamTemplate::Box2DObject {
public:
    Player(StateGame& sg, const b2BodyDef* def);

    void setB2Obj();

private:
    StateGame& m_gameState;

    JamTemplate::Animation::Sptr m_sprite;

    JamTemplate::SmartShape::Sptr m_collider;
    bool m_drawCollider { true };

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();
};

#endif
