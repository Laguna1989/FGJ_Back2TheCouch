
#include "Level.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "JamTemplate/Random.hpp"
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
    // border
    for (int i = -1; i != static_cast<int>(levelImage.getSize().x); ++i) {
        AddTile(i, -1, TileType::TileType::Platform);
        AddTile(i, levelImage.getSize().y, TileType::TileType::Platform);
    }
    for (int j = -1; j != static_cast<int>(levelImage.getSize().y); ++j) {
        AddTile(-1, j, TileType::TileType::Platform);
        AddTile(levelImage.getSize().x, j, TileType::TileType::Platform);
    }
    // image
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

sf::Vector2f Level::getSpawnPositionAbove(float minHeight) const
{
    std::vector<sf::Vector2i> vec;
    std::copy_if(m_spawnPositions.begin(), m_spawnPositions.end(), std::back_inserter(vec), [minHeight](auto const& v) {
        return v.y < ((minHeight / GP::SpriteSize()) - 1);
    });
    std::sort(vec.begin(), vec.end(), [](auto const& a, auto const& b) -> bool { return a.y > b.y; });
    // If there are multiple lowest spawn points at the same height, pick one at random.
    auto targetHeight = vec.at(0).y;
    std::vector<sf::Vector2i> allLowest;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(allLowest), [targetHeight](auto const& v) {
        return v.y == targetHeight;
    });
    int chosenIndex = JamTemplate::Random::getInt(0, allLowest.size() - 1);
    return sf::Vector2f { static_cast<float>(GP::SpriteSize() * allLowest.at(chosenIndex).x), static_cast<float>(GP::SpriteSize() * allLowest.at(chosenIndex).y) };
}
