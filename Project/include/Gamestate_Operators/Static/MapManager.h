#pragma once
#include <SFML/Graphics.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "Literals.h"

/**
 * @author Mar Werner Hernandez 
 * 
 */

/**
 * @brief Interface to work with the parallax background
 * 
 */
class MapManager : public StaticInitializableGamestateOperator<MapManager>, public StaticUpdatableGamestateOperator<MapManager>, public StaticDrawableGamestateOperator<MapManager> {
    
    struct ParallaxBackground {
        static inline constexpr u32 NUM_PARALLAX_LAYERS = 5;

        sf::Texture t_layers[NUM_PARALLAX_LAYERS];
        sf::Sprite s_layers[NUM_PARALLAX_LAYERS];

        float intermediateTextureRectOffsets[NUM_PARALLAX_LAYERS] = {};
    };

    /**
     * @brief Gets the ParallaxBackground singleton
     * 
     * @return ParallaxBackground& 
     */
    static ParallaxBackground& GetBackground(void);
    
    public:
    enum class Maps : u8 {CITY, PARK, PLAINS, NUM_MAPS};

    /**
     * @brief Sets the map to a default (the city)
     * 
     */
    static void Initialize(void);
    
    /**
     * @brief Draws the parallax background and wraps the textures according to the camera's position
     * 
     * @param window 
     */
    static void Draw(sf::RenderWindow& window);

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
};