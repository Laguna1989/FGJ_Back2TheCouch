
#include "Level.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "StateGame.hpp"
#include <iostream>

Level::Level(StateGame& sg)
    : m_gameState(sg)
{
    m_tiles = std::make_shared<JamTemplate::ObjectGroup<Tile>>();
}

void Level::LoadLevel(std::string const& fileName)
{
    auto levelImage = JamTemplate::TextureManager::get(fileName).copyToImage();

    for (unsigned int x = 0; x != levelImage.getSize().x; ++x) {
        for (unsigned int y = 0; y != levelImage.getSize().y; ++y) {
            sf::Color c = levelImage.getPixel(x, y);

            if (c == sf::Color::White) {
                AddTile(x, y, TileType::TileType::Platform);
            }
            if (c.r == 1 && c.g == 255 && c.b == 0) {
                m_spawnPositions.emplace_back(sf::Vector2i { static_cast<int>(x), static_cast<int>(y) });
            }
        }
    }
    for (int i = 0; i != levelImage.getSize().x; ++i) {
        AddTile(i, 0, TileType::TileType::Platform);
        AddTile(i, levelImage.getSize().y, TileType::TileType::Platform);
    }
    for (int j = 0; j != levelImage.getSize().y; ++j) {
        AddTile(0, j, TileType::TileType::Platform);
        AddTile(levelImage.getSize().x, j, TileType::TileType::Platform);
    }
}

void Level::AddTile(int x, int y, TileType::TileType type)
{
    b2BodyDef wallBodyDef;
    wallBodyDef.fixedRotation = true;
    wallBodyDef.allowSleep = false;

    std::shared_ptr<Tile> t = std::make_shared<Tile>(x, y, TileType::TileType::Platform, &wallBodyDef, m_gameState.getWorld());
    m_tiles->emplace_back(t);
    m_gameState.add(t);
}

void Level::doCreate()
{
    m_backgroundImage = std::make_shared<JamTemplate::SmartSprite>();
    m_backgroundImage->loadSprite("assets/background.png");
}

void Level::doUpdate(float const elapsed)
{
    m_tiles->update(elapsed);
    m_backgroundImage->update(elapsed);
}
void Level::doDraw() const
{
    //m_backgroundImage->draw(getGame()->getRenderTarget());
}

sf::Vector2f Level::getSpawnPosition(int id) const
{
    id = id % m_spawnPositions.size();

    auto pi = m_spawnPositions.at(id);
    return sf::Vector2f { static_cast<float>(GP::SpriteSize() * pi.x), static_cast<float>(GP::SpriteSize() * pi.y) };
}
