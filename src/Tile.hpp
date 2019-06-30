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
    PlatformFull,
    PlatformHalf,
    PlatformQuart,
};
}

class Tile : public JamTemplate::Box2DObject {

public:
    Tile(int x, int y, TileType::TileType ty, const b2BodyDef* def, std::shared_ptr<b2World> world)
        : JamTemplate::Box2DObject { world, def }
        , m_x(x)
        , m_y(y)
        , m_type { ty }
    {
    }
    ~Tile() = default;

    int X() { return m_x; }
    int Y() { return m_y; }

    virtual void SetTexture()
    {
        m_sprite->loadSprite("assets/tile.png");
        if (m_type == TileType::TileType::PlatformHalf) {
            m_sprite->setScale(sf::Vector2f { 1, 0.5f });
        } else if (m_type == TileType::TileType::PlatformQuart) {
            m_sprite->setScale(sf::Vector2f { 1, 0.25f });
        }
    }

    TileType::TileType Type() { return m_type; }

    JamTemplate::SmartSprite::Sptr getSprite() { return m_sprite; };

private:
    sf::Vector2f m_HitBoxOffset { 0, 0 };
    sf::Vector2f m_offsetSpr { 0, 0 };
    void updatePosition()
    {
        setPosition(m_offsetSpr + sf::Vector2f(static_cast<float>(m_x * GP::SpriteSize()), static_cast<float>(m_y * static_cast<int>(GP::SpriteSize()))));
        m_sprite->setPosition(getPosition() - m_HitBoxOffset);
    }

    void doCreate() override
    {
        m_sprite = std::make_shared<JamTemplate::SmartSprite>();

        SetTexture();
        setB2Obj();

        updatePosition();
        m_sprite->setColor(sf::Color { 0, 0, 0, 0 });
    }

    void doUpdate(const float elapsed) override
    {
        m_sprite->update(elapsed);
        if (JamTemplate::InputManager::justPressed(sf::Keyboard::Key::F6)) {
            if (m_sprite->getColor() == sf::Color::White) {
                m_sprite->setColor(sf::Color { 0, 0, 0, 0 });
            } else {
                m_sprite->setColor(sf::Color::White);
            }
        }
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
        float32 ss = static_cast<float32>(GP::SpriteSize());

        if (m_type == TileType::TileType::PlatformHalf) {
            dynamicBox.SetAsBox(ss / 4.0f, ss / 4.0f);
            m_HitBoxOffset = sf::Vector2f { 0, -ss / 8 };
            m_offsetSpr = sf::Vector2f { 0, -2 };

        } else if (m_type == TileType::TileType::PlatformQuart) {
            dynamicBox.SetAsBox(ss / 4.0f, ss / 4.0f);
            m_offsetSpr = sf::Vector2f { 0, -2 };
            m_HitBoxOffset = sf::Vector2f { 0, -ss / 8 };
        } else {
            dynamicBox.SetAsBox(ss / 2.0f, ss / 2.0f);
        }

        b2FixtureDef fixtureDef;

        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = GP::TileDensity();
        fixtureDef.friction = GP::TileFriction();

        getB2Body()->CreateFixture(&fixtureDef);
        getB2Body()->SetLinearDamping(GP::SpriteLinearDamping());
    }
};

#endif
