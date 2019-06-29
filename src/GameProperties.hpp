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

    static unsigned int PlayfieldWidth() { return 800; }
    static unsigned int PlayfieldHeight() { return 600; }
    static unsigned int PixelScalingFactor() { return 3; }

    static sf::Color PaletteBackground() { return sf::Color { 14, 16, 20 }; };
    static sf::Color PaletteFontFront() { return sf::Color { 248, 249, 254 }; };
    static sf::Color PaletteFontShadow(sf::Uint8 a = 255) { return sf::Color { 26, 29, 36, a }; };
    static sf::Color PaletteSelector() { return sf::Color { 221, 228, 236 }; };
    static sf::Color PaletteHighlight() { return sf::Color { 163, 183, 205 }; };

    static sf::Color PaletteFlashGreen() { return sf::Color { 51, 143, 63 }; };
    static sf::Color PaletteFlashRed() { return sf::Color { 143, 33, 33 }; };

    static std::string GameName() { return "Back 2 The Couch"; };

    static std::int32_t PhysicVelocityIterations() { return 8; };
    static std::int32_t PhysicPositionIterations() { return 2; };
    static float PhysicsGravity() { return 1200.0f; };

    static unsigned int SpriteSize() { return 12; };
    static float SpriteLinearDamping() { return 0.085f; };

    static float PlayerMovementAcceleration() { return 65000.0f; };
    static float PlayerFriction() { return 0.01f; };
    static float PlayerDensity() { return 1.0f * 6.0f / 3.5f; };
    static float PlayerMaxSpeedHorizontal() { return 125.0f; };
    static float PlayerMaxSpeedVertical() { return 500.0f; };
    static float PlayerJumpImpulse() { return 85000.0f; };
    static float PlayerVerticalSpeedJumpThreshold() { return 1e-4f; };

    static float TileFriction() { return 0.48f; };
    static float TileDensity() { return 1.0f; };


    static float ShotVelocity() { return 250; };
    static float ShotTimer() { return 0.25; };
    static float ShotInvincibleTime() { return 0.1f; };

    static float LavaRiseSpeed() { return 5.0f; };
    static float LavaStartTime() { return 2.0f; };
    static float LavaStopTime() { return 27.0f; };
    static float TotalGameTime() { return 50.0f; };
};

#endif
