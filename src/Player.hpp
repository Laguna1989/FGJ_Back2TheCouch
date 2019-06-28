#ifndef PLAYER_HPP_INCLUDEGUARD
#define PLAYER_HPP_INCLUDEGUARD

#include "JamTemplate/Animation.hpp"
#include "JamTemplate/GameObject.hpp"

class Player : public JamTemplate::GameObject {
public:
    Player();

private:
    JamTemplate::Animation::Sptr m_sprite;

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();
};

#endif
