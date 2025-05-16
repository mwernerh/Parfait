#pragma once
#include <SFML/Graphics/Text.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"

/**
 * @author Mar Werner Hernandez
 * 
 */

 /**
  * @brief Operator notifying player that thet've run out of health
  * 
  */
class GameOverTextOperator : public InstancedDrawableGamestateOperator {
    sf::Text gameoverText;

    public:
    GameOverTextOperator();
    void Draw(sf::RenderWindow& window) override;
};