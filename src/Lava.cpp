#include "Lava.hpp"
#include "JamTemplate/Game.hpp"

Lava::Lava(StateGame& sg, int id)
    : m_gameState(sg)
    , m_id { id }
{
}

void Lava::doCreate()
{
    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    m_lavaShape = std::make_shared<JamTemplate::SmartShape>();
    m_lavaShape->makeRect(sf::Vector2f { w, h });
    m_lavaShape->setColor(sf::Color { 184, 95, 0, 200 });
    m_lavaShape->setPosition(sf::Vector2f { 0.0f, h });
    m_lavaShape->update(0.0f);
}

void Lava::doUpdate(float const elapsed)
{
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    const float age = getAge();
    if (age > GP::LavaStartTime() && age < GP::LavaStopTime()) {
        m_lavaShape->setPosition(sf::Vector2f { 0.0f, h - (getAge() * GP::LavaRiseSpeed()) + (GP::LavaStartTime() * GP::LavaRiseSpeed()) });
    }
    m_lavaShape->update(elapsed);
}

void Lava::doDraw() const
{
    m_lavaShape->draw(getGame()->getRenderTarget());
}
