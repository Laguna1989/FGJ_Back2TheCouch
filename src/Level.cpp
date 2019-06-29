
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
}

void Level::doCreate()
{
}

void Level::doUpdate(float const elapsed)
{
}
void Level::doDraw() const
{
}
