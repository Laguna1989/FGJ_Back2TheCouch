#ifndef LAVA_HPP_INCLUDEGUARD
#define LAVA_HPP_INCLUDEGUARD

#include "GameProperties.hpp"
#include "JamTemplate/Animation.hpp"
#include "JamTemplate/Box2DObject.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "PlayerInput.hpp"
#include <Box2D/Box2D.h>

// forward declaration
class StateGame;

class Lava : public JamTemplate::GameObject {
public:
    Lava(StateGame& sg, int id);

    float getHeight() const;
    JamTemplate::SmartShape::Sptr getSprite() { return m_lavaShape; };

private:
    StateGame& m_gameState;
    int m_id { 0 };

    JamTemplate::SmartShape::Sptr m_lavaShape;

    virtual void doCreate();
    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
};

#endif
