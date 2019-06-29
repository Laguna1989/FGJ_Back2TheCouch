#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include <SFML/Graphics/Color.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static float CellPositionSpacing() { return 32.0f; }
    static float CellPositionOffsetX() { return 10.0f; }
    static float CellPositionOffsetY() { return 10.0f; }
    static float GameBoardInputTimerMax() { return 0.6f; }
    static float GameBoardInputOffsetMax() { return 300; }
    static unsigned int GameBoardMaxParse() { return 20; }

    static sf::Color PaletteBackground() { return sf::Color { 14, 16, 20 }; };
    static sf::Color PaletteFontFront() { return sf::Color { 248, 249, 254 }; };
    static sf::Color PaletteFontShadow(sf::Uint8 a = 255) { return sf::Color { 26, 29, 36, a }; };
    static sf::Color PaletteSelector() { return sf::Color { 221, 228, 236 }; };
    static sf::Color PaletteHighlight() { return sf::Color { 163, 183, 205 }; };

    static sf::Color PaletteFlashGreen() { return sf::Color { 51, 143, 63 }; };
    static sf::Color PaletteFlashRed() { return sf::Color { 143, 33, 33 }; };

    static std::string GameName() { return "Back 2 The Couch"; };

    static std::int32_t PhysicVelocityIterations() { return 2; };
    static std::int32_t PhysicPositionIterations() { return 2; };
    static float PhysicsGravity() { return 500; };

    static unsigned int SpriteSize() { return 12; };
    static float SpriteLinearDamping() { return 0.1f; };

    static float PlayerMovementAcceleration() { return 20000; };
    static float PlayerFriction() { return 0.9f; };
    static float PlayerDensity() { return 1.0f; };

    static float TileFriction() { return 0.9f; };
    static float TileDensity() { return 1.0f; };
};

#endif
