#pragma once
#include "Gamestate_Operators/Instanced/GameOverTextOperator.h"
#include "Gamestates/BaseGameState.h"

struct GameOverGameState : public InstancedGameState<GameOverGameState, GameOverTextOperator> {
    GameOverGameState() : InstancedGameState() {}
    void Update(void);
    void Draw(sf::RenderWindow& window);
};