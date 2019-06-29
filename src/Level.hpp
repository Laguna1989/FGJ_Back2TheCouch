#ifndef LEVEL_HPP_INCLUDEGUARD
#define LEVEL_HPP_INCLUDEGUARD

#include "JamTemplate/GameObject.hpp"
#include "JamTemplate/SmartSprite.hpp"

// forward declaration
class StateGame;

class Level : public JamTemplate::GameObject {
public:
    Level(StateGame& sg);

    void LoadLevel(std::string const& fileName);

private:
    StateGame& m_gameState;
    JamTemplate::SmartSprite::Sptr m_backgroundImage;

    virtual void doUpdate(float const /*elapsed*/);
    virtual void doDraw() const;
    virtual void doCreate();
};

#endif
