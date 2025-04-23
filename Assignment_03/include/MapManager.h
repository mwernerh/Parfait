#pragma once
#include <SFML/Graphics.hpp>
#include "Literals.h"
#include "EnvironmentCollision.h"

class MapManager {
    
    struct ParallaxBackground {
        static inline constexpr u32 NUM_PARALLAX_LAYERS = 5;

        sf::Texture t_layers[NUM_PARALLAX_LAYERS];
        sf::Sprite s_layers[NUM_PARALLAX_LAYERS];
    };

    /**
     * @brief Gets the ParallaxBackground singleton
     * 
     * @return ParallaxBackground& 
     */
    static ParallaxBackground& GetBackground(void);
    
    public:
    enum class Maps : u8 {CITY, PARK, CITY_VAR, NUM_MAPS};
    
    /**
     * @brief Draws the parallax background and wraps the textures according to the player's position and velocity
     * 
     * @param window 
     * @param player_position
     * @param player_horizontal_velocity 
     */
    static void Draw(sf::RenderWindow& window, sf::Vector2f player_position, float player_horizontal_velocity);

    /**
     * @brief Currently No-Op
     * 
     */
    static void Update(void);

    /**
     * @brief Changes the parallax background to that associated with the desired map
     * 
     */
    static void ChangeMap(const Maps);

    /**
     * @brief Gets the EnvironmentCollision singleton (current unused)
     * 
     * @return EnvironmentCollision& 
     */
    static EnvironmentCollision& GetCollision(void);
};