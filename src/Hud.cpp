#include "Hud.hpp"

#include "GameProperties.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/MathHelper.hpp"

Hud::Hud() = default;

void Hud::AddScoreP1(int i)
{
    m_scoreP1 += i;
    if (m_scoreP1 < 0) {
        m_scoreP1 = 0;
    }
}
void Hud::AddScoreP2(int i)
{
    m_scoreP2 += i;
    if (m_scoreP2 < 0) {
        m_scoreP2 = 0;
    }
}
void Hud::doUpdate(float const elapsed)
{
    using MH = JamTemplate::MathHelper;

    m_scoreP1Text->setText(std::to_string(m_scoreP1));
    m_scoreP2Text->setText(std::to_string(m_scoreP2));
    m_scoreP1Text->update(elapsed);
    m_scoreP2Text->update(elapsed);
    int secondsLeft = static_cast<int>(GP::TotalGameTime() - getAge());
    m_timerText->setText(std::to_string(MH::clamp(secondsLeft, 0, INT_MAX)));
    m_timerText->update(elapsed);
    float center = GP::PlayfieldWidth() / (2.0f * GP::PixelScalingFactor());
    auto textBoundingBox = m_timerText->getGlobalBounds();
    m_timerText->setPosition({ center /*- (textBoundingBox.width / 2.0f)*/, 10 });
}

void Hud::doDraw() const
{
    m_scoreP1Text->draw(getGame()->getRenderTarget());
    m_scoreP2Text->draw(getGame()->getRenderTarget());
    if (m_drawTimer) {
        m_timerText->draw(getGame()->getRenderTarget());
    }
}

void Hud::doCreate()
{
    m_scoreP1Text = std::make_shared<JamTemplate::SmartText>();
    m_scoreP1Text->loadFont("assets/font.ttf");
    m_scoreP1Text->setCharacterSize(16U);
    m_scoreP1Text->setColor(sf::Color { 248, 249, 254 });
    m_scoreP1Text->setOutline(2, GP::Player1SecondaryColor());
    m_scoreP1Text->update(0.0f);
    m_scoreP1Text->SetTextAlign(JamTemplate::SmartText::TextAlign::LEFT);
    m_scoreP1Text->setPosition({ 8.0f, 0 });

    m_scoreP2Text = std::make_shared<JamTemplate::SmartText>();
    m_scoreP2Text->loadFont("assets/font.ttf");
    m_scoreP2Text->setCharacterSize(16U);
    m_scoreP2Text->setColor(sf::Color { 248, 249, 254 });
    m_scoreP2Text->setOutline(2, GP::Player2SecondaryColor());
    m_scoreP2Text->update(0.0f);
    m_scoreP2Text->SetTextAlign(JamTemplate::SmartText::TextAlign::RIGHT);
    m_scoreP2Text->setPosition({ getGame()->getRenderTarget()->getSize().x - 4.0f, 0 });

    m_timerText = std::make_shared<JamTemplate::SmartText>();
    m_timerText->loadFont("assets/font.ttf");
    m_timerText->setCharacterSize(16U);
    m_timerText->setColor(sf::Color { 248, 249, 254 });
    m_timerText->setOutline(2, sf::Color { 60, 60, 70 });
    m_timerText->SetTextAlign(JamTemplate::SmartText::TextAlign::CENTER);
    m_timerText->update(0.0f);
}
