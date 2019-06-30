#ifndef SHOT_HPP_INCLUDEGUARD
#define SHOT_HPP_INCLUDEGUARD

#include "JamTemplate/Animation.hpp"
#include "JamTemplate/GameObject.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/Transform.hpp"
#include "PlayerInput.hpp"
#include <Box2D/Box2D.h>

class StateGame;

class Shot : public JamTemplate::GameObject, public JamTemplate::Transform {
public:
    Shot(StateGame& sg);

    void Start2Move()
    {
        m_movementStarted = true;
        m_sprite->play("idle", 0, true);
    };
    void Collide()
    {
        if (!m_collided) {
            m_collided = true;
            m_sprite->play("clash", 0, true);
            setAge(5 - 3 * 0.075f);
        }
    }

    bool isShotInValid()
    {
        return (!isAlive() || !m_movementStarted || m_collided);
    }

    JamTemplate::Animation::Sptr getSprite();

    bool m_collided { false };
    bool m_movementStarted { false };

    int m_firedBy { 0 };

private:
    JamTemplate::Animation::Sptr m_sprite;

    StateGame& m_stateGame;

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();
};

#endif
