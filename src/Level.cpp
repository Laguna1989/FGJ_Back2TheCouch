
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

    for (int i = 0; i != 10; ++i) {
        AddTile(i, 5, TileType::TileType::Platform);
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
