#include "StateMenu.hpp"
#include "GameProperties.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartText.hpp"
#include "JamTemplate/TextureManager.hpp"
#include "JamTemplate/TweenAlpha.hpp"
#include "JamTemplate/TweenColor.hpp"
#include "JamTemplate/TweenPosition.hpp"
#include "JamTemplate/TweenScale.hpp"
#include "StateGame.hpp"

StateMenu::StateMenu() = default;
void StateMenu::doInternalUpdate(float const elapsed)
{
    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    float wC = w / 2;
    auto p = sf::Vector2f { wC, h / 2 - 24 } + sf::Vector2f { 0, (std::sin(getAge()) * std::sin(getAge())) * 10 };
    m_title->setPosition(p);
    m_title->update(elapsed);

    m_text_start->update(elapsed);
    m_text_start2->update(elapsed);
    if (!m_starting) {
        using ip = JamTemplate::InputManager;
        if (ip::justPressed(sf::Keyboard::Key::Space) || ip::justPressed(sf::Keyboard::Key::Return)) {

            auto func = [](int b) { int a = 5;  a+=b; };

            m_starting = true;
            auto tw = JamTemplate::TweenAlpha<JamTemplate::SmartShape>::create(m_overlay, 0.5f, sf::Uint8 { 0 }, sf::Uint8 { 255 });
            tw->setSkipFrames();
            tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateGame>()); });
            add(tw);
        }
    }
}

void StateMenu::doCreate()
{
    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    float wC = w / 2;

    m_background = std::make_shared<JamTemplate::SmartSprite>();
    m_background->loadSprite("assets/titlescreen.png");
    m_background->update(0.0f);

    m_title = std::make_shared<JamTemplate::SmartSprite>();
    m_title->loadSprite("assets/grounded.logo.png");
    m_title->setPosition(sf::Vector2f { wC, h / 2 });
    m_title->setOffset(sf::Vector2f { -m_title->getLocalBounds().width / 2, -m_title->getLocalBounds().height / 2 });
    m_title->update(0.0f);

    m_text_start = std::make_shared<JamTemplate::SmartSprite>();
    m_text_start->loadSprite("assets/star_textt.png");
    m_text_start->setPosition(sf::Vector2f { wC, h / 2 + 24 });
    m_text_start->setOffset(sf::Vector2f { -m_text_start->getLocalBounds().width / 2, -m_text_start->getLocalBounds().height / 2 });
    m_text_start->update(0.0f);

    m_text_start2 = std::make_shared<JamTemplate::SmartSprite>();
    m_text_start2->loadSprite("assets/star_textt.png");
    m_text_start2->setColor(sf::Color { 0, 0, 0 });
    m_text_start2->setPosition(sf::Vector2f { wC, h / 2 + 24 });
    m_text_start2->setOffset(sf::Vector2f { -m_text_start->getLocalBounds().width / 2 + 1, -m_text_start->getLocalBounds().height / 2 + 1 });
    m_text_start2->update(0.0f);

    auto tw = JamTemplate::TweenColor<JamTemplate::SmartSprite>::create(m_text_start, 0.5f, sf::Color::White, sf::Color { 100, 100, 100 });
    tw->setRepeat(true);
    add(tw);

    m_overlay = std::make_shared<JamTemplate::SmartShape>();
    m_overlay->makeRect(sf::Vector2f { w, h });
    m_overlay->setColor(sf::Color { 0, 0, 0 });
    m_overlay->update(0);

    {
        auto tw = JamTemplate::TweenAlpha<JamTemplate::SmartShape>::create(m_overlay, 0.5f, sf::Uint8 { 255 }, sf::Uint8 { 0 });
        tw->setSkipFrames();
        add(tw);
    }
}
void StateMenu::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());

    m_title->draw(getGame()->getRenderTarget());

    m_text_start2->draw(getGame()->getRenderTarget());
    m_text_start->draw(getGame()->getRenderTarget());

    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float wC = w / 2;

    m_overlay->draw(getGame()->getRenderTarget());
}
