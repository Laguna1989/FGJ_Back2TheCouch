#include "GameProperties.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/TextureManager.hpp"
#include "StateMenu.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    JamTemplate::Game::Sptr game = std::make_shared<JamTemplate::Game>();
    game->setUp(GP::PlayfieldWidth(), GP::PlayfieldHeight(), GP::PixelScalingFactor(), GP::GameName());

    auto window = game->getRenderWindow();
    auto renderTexture = game->getRenderTarget();
    auto view = game->getView();

    game->switchState(std::make_shared<StateMenu>());
    sf::Clock clock;

    JamTemplate::TextureManager::addselectiveColorReplacement(0, { std::make_pair(sf::Color { 23, 255, 0 }, GP::Player1PrimaryColor()), std::make_pair(sf::Color { 255, 0, 139 }, GP::Player1SecondaryColor()), std::make_pair(sf::Color { 101, 0, 55 }, GP::Player1TertiaryColor()) });
    JamTemplate::TextureManager::addselectiveColorReplacement(1, { std::make_pair(sf::Color { 23, 255, 0 }, GP::Player2PrimaryColor()), std::make_pair(sf::Color { 255, 0, 139 }, GP::Player2SecondaryColor()), std::make_pair(sf::Color { 101, 0, 55 }, GP::Player2TertiaryColor()) });

    sf::Music music;
    if (!music.openFromFile("assets/music_loop.ogg")) {
        std::cerr << "cannot load music file!\n";
    }
    music.setLoop(true);
    music.play();

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
