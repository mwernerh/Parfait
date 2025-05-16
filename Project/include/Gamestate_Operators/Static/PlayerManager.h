#pragma once

#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "Player.h"

/**
 * @author Mar Werner Hernandez 
 * @author Izzy Carlson
 * 
 */

/**
 * @brief Interface to the static player instance
 * 
 */
class PlayerManager : public StaticInitializableGamestateOperator<PlayerManager>, public StaticUpdatableGamestateOperator<PlayerManager>, public StaticDrawableGamestateOperator<PlayerManager> {

    /**
     * @brief Get a modifiable reference to the Player instance
     * 
     * @return Player& 
     */
    static Player& GetPlayerInternal(void);
    
    public:
    /**
     * @brief Get an unmodifiable reference to the Player instance
     * 
     * @return const Player& 
     */
    static const Player& GetPlayer(void);

    /**
     * @brief Set the player's health
     * 
     * @param health 
     */
    static void SetPlayerHealth(const s32 health);

    /**
     * @brief Creates the player instance
     * 
     */
    static void Initialize(void);

    /**
     * @brief Updates the player
     * 
     */
    static void Update(void);

    /**
     * @brief Draws the player
     * 
     * @param window 
     */
    static void Draw(sf::RenderWindow& window);
};