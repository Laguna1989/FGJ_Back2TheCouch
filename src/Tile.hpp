#ifndef GAME_TILE_HPP_INCLUDEGUARD
#define GAME_TILE_HPP_INCLUDEGUARD

#include "JamTemplate/Game.hpp"
#include "JamTemplate/GameObject.hpp"
#include "JamTemplate/SmartSprite.hpp"
#include "JamTemplate/Transform.hpp"

#include "GameProperties.hpp"
#include "JamTemplate/TextureManager.hpp"

#include "JamTemplate/Box2DObject.hpp"

namespace TileType {
enum TileType {
    Platform
};
}

class Tile : public JamTemplate::Box2DObject {

public:
    Tile(int x, int y, TileType::TileType ty, const b2BodyDef* def, std::shared_ptr<b2World> world)
        : JamTemplate::Box2DObject { world, def }
        , m_x(x)
        , m_y(y)
    {
    }
    ~Tile() = default;

    int X() { return m_x; }
    int Y() { return m_y; }

    virtual void SetTexture() { m_sprite->loadSprite("assets/tile.png"); }

    TileType::TileType Type() { return m_type; }

private:
    void updatePosition()
    {
        setPosition(sf::Vector2f(static_cast<float>(m_x * GP::TileSpriteSize()), static_cast<float>(m_y * GP::TileSpriteSize())));
        m_sprite->setPosition(getPosition());
    }

    void doCreate() override
    {
        m_sprite = std::make_shared<JamTemplate::SmartSprite>();

        SetTexture();
        setB2Obj();

        updatePosition();
    }

    void doUpdate(const float elapsed) override
    {
        m_sprite->update(elapsed);
    }

    void doDraw() const override
    {
        m_sprite->draw(getGame()->getRenderTarget());
    }

protected:
    int m_x;
    int m_y;

    TileType::TileType m_type;

    JamTemplate::SmartSprite::Sptr m_sprite;

private:
    void setB2Obj()
    {
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(GP::TileSpriteSize() / 2, GP::TileSpriteSize() / 2);

        b2FixtureDef fixtureDef;

        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.9f;

        getB2Body()->CreateFixture(&fixtureDef);
        getB2Body()->SetLinearDamping(0.1f);
    }
};

#endif
