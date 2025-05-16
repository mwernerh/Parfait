#pragma once
#include "Gamestate_Operators/Instanced/PauseMenuOperator.h"
#include "Gamestates/BaseGameState.h"

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Gamestate that is switched to when the player wants to pause
 * 
 */
struct PauseGameState : public InstancedGameState<PauseGameState, PauseMenuOperator> {
    PauseGameState() : InstancedGameState() {}
    void Update(void);
    void Draw(sf::RenderWindow& window);
};