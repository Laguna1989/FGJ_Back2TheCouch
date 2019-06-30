#ifndef COUCH_HPP_INCLUDEGUARD
#define COUCH_HPP_INCLUDEGUARD

#include "JamTemplate/Animation.hpp"
#include "JamTemplate/GameObject.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/Transform.hpp"
#include "PlayerInput.hpp"
#include <Box2D/Box2D.h>

// forward declaration
class StateGame;
class Shot;

class Couch : public JamTemplate::GameObject, public JamTemplate::Transform {
public:
    Couch(StateGame& sg, const b2BodyDef* def);

    JamTemplate::SmartSprite::Sptr getSprite() { return m_sprite; };

private:
    StateGame& m_gameState;

    JamTemplate::SmartSprite::Sptr m_sprite;

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();

    void updateMovement(float const elapsed);
};

#endif
