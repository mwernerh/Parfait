#pragma once
#include "Gamestate_Operators/Instanced/PauseMenuOperator.h"
#include "Gamestates/BaseGameState.h"

struct PauseGameState : public InstancedGameState<PauseGameState, PauseMenuOperator> {
    PauseGameState() : InstancedGameState() {}
    void Update(void);
    void Draw(sf::RenderWindow& window);
};