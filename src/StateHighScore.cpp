#include "StateHighScore.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "JamTemplate/Animation.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartText.hpp"
#include "JamTemplate/TextureManager.hpp"
#include "JamTemplate/TweenAlpha.hpp"
#include "JamTemplate/TweenPosition.hpp"
#include "JamTemplate/TweenScale.hpp"
#include "StateMenu.hpp"

StateHighscore::StateHighscore(std::shared_ptr<Hud> hud)
    : m_hud { hud }
{
}

void StateHighscore::doCreate()
{
    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    float wC = w / 2;

    m_backgroundYellow = std::make_shared<JamTemplate::SmartShape>();
    m_backgroundYellow->makeRect(sf::Vector2f(w, h));
    m_backgroundYellow->setColor(sf::Color { 255, 243, 146 });

    m_backgroundRed = std::make_shared<JamTemplate::SmartShape>();
    m_backgroundRed->makeRect(sf::Vector2f(w, 13));
    m_backgroundRed->setColor(sf::Color { 255, 17, 0 });
    m_backgroundRed->setPosition(sf::Vector2f { 0, h - 13 });
    m_backgroundRed->update(0.0f);

    for (int i = 0; i != 3; ++i) {
        std::shared_ptr<JamTemplate::Animation> anim = std::make_shared<JamTemplate::Animation>();
        anim->add("assets/Endscreen_Low.png", "idle", sf::Vector2u { 96, 15 }, JamTemplate::MathHelper::vectorBetween(0U, 22U), 0.2f);
        anim->play("idle", JamTemplate::Random::getInt(0, 22));
        anim->setPosition(sf::Vector2f { i * anim->getLocalBounds().width, h - anim->getLocalBounds().height });
        m_backgroundAnims.push_back(anim);
    }

    std::shared_ptr<JamTemplate::Animation> anim_main = std::make_shared<JamTemplate::Animation>();
    anim_main->add("assets/Endscreen.png", "idle", sf::Vector2u { 96, 50 }, JamTemplate::MathHelper::vectorBetween(0U, 22U), 0.2f);
    anim_main->play("idle");
    anim_main->setPosition(sf::Vector2f { wC - anim_main->getLocalBounds().width / 2.0f, h - anim_main->getLocalBounds().height });
    m_backgroundAnims.push_back(anim_main);

    add(m_hud);
    m_hud->DisableTimer();
    m_hud->setReturnToCouchTextVisible(false);

    //m_playerWin   = std::make_shared;
    //m_playerLoose = std::make_shared;
}

void StateHighscore::doInternalUpdate(float const elapsed)
{
    m_backgroundYellow->update(elapsed);
    m_backgroundRed->update(elapsed);

    for (auto a : m_backgroundAnims) {
        a->update(elapsed);
    }

    if (!m_starting) {
        using ip = JamTemplate::InputManager;
        if (ip::justPressed(sf::Keyboard::Key::Space) || ip::justPressed(sf::Keyboard::Key::Return)) {

            auto func = [](int b) { int a = 5;  a+=b; };

            m_starting = true;
            auto tw = JamTemplate::TweenAlpha<JamTemplate::SmartShape>::create(m_overlay, 0.5f, sf::Uint8 { 0 }, sf::Uint8 { 255 });
            tw->setSkipFrames();
            tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateMenu>()); });
            add(tw);
        }
    }
}

void StateHighscore::doInternalDraw() const
{
    m_backgroundYellow->draw(getGame()->getRenderTarget());
    m_backgroundRed->draw(getGame()->getRenderTarget());
    for (auto a : m_backgroundAnims) {
        a->draw(getGame()->getRenderTarget());
    }
    m_hud->draw();
}
