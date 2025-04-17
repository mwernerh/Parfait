#pragma once
#include <SFML/Graphics.hpp>
#include <Literals.h>

class MapManager {
    
    struct ParallaxBackground {
        static inline constexpr u32 NUM_PARALLAX_LAYERS = 5;

        sf::Texture t_layers[NUM_PARALLAX_LAYERS];
        sf::Sprite s_layers[NUM_PARALLAX_LAYERS];
    };

    // SFML doesn't support static variable initialization for its provided types
    // Wrap the static background variable in a function to circumvent this
    static ParallaxBackground& GetBackground(void);
    
    public:
    enum class Maps : u8 {CITY, PARK, CITY_VAR, NUM_MAPS};

    static void Draw(sf::RenderWindow& window, sf::Vector2f playerPos, float playerHorizontalVelocity);
    static void ChangeMap(const Maps);
};