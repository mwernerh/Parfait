#pragma once
#include "Gamestate_Operators/Instanced/GameOverTextOperator.h"
#include "Gamestates/BaseGameState.h"

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Gamestate that is switched to when the player loses all their health
 * 
 */
struct GameOverGameState : public InstancedGameState<GameOverGameState, GameOverTextOperator> {
    GameOverGameState() : InstancedGameState() {}
    void Update(void);
    void Draw(sf::RenderWindow& window);
};