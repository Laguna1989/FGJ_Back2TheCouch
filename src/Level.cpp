
#include "Level.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "StateGame.hpp"
#include <iostream>

Level::Level(StateGame& sg)
    : m_gameState(sg)
{
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
}

void Level::AddTile(int x, int y, TileType::TileType type)
{
    b2BodyDef wallBodyDef;
    wallBodyDef.fixedRotation = true;
    wallBodyDef.allowSleep = false;

    std::shared_ptr<Tile> t = std::make_shared<Tile>(x, y, TileType::TileType::Platform, &wallBodyDef, m_gameState.getWorld());
    m_tiles.emplace_back(t);
    m_gameState.add(t);
}

void Level::doCreate()
{
}

void Level::doUpdate(float const elapsed)
{
    m_tiles.update(elapsed);
}
void Level::doDraw() const
{
}

sf::Vector2f Level::getSpawnPosition(int id) const
{
    id = id % m_spawnPositions.size();

    auto pi = m_spawnPositions.at(id);
    return sf::Vector2f { static_cast<float>(GP::SpriteSize() * pi.x), static_cast<float>(GP::SpriteSize() * pi.y) };
}
