#include "GameProperties.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/TextureManager.hpp"
#include "StateMenu.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    JamTemplate::Game::Sptr game = std::make_shared<JamTemplate::Game>();
    game->setUp(800, 600, 3, GP::GameName());

    auto window = game->getRenderWindow();
    auto renderTexture = game->getRenderTarget();
    auto view = game->getView();

    game->switchState(std::make_shared<StateMenu>());
    sf::Clock clock;

    JamTemplate::TextureManager::addselectiveColorReplacement(0, { std::make_pair(sf::Color { 23, 255, 0 }, sf::Color { 0, 0, 255 }) });
    JamTemplate::TextureManager::addselectiveColorReplacement(1, { std::make_pair(sf::Color { 23, 255, 0 }, sf::Color { 255, 0, 0 }) });

    while (window->isOpen()) {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        game->update(elapsed.asSeconds());
        game->draw();
    }

    return 0;
}
