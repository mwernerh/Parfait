#pragma once
#include <SFML/Graphics/Text.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"
/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Operator notifying player than the game is paused
 * 
 */
class PauseMenuOperator : public InstancedDrawableGamestateOperator {
    sf::Text pauseText;

    public:
    PauseMenuOperator();
    void Draw(sf::RenderWindow& window) override;
};