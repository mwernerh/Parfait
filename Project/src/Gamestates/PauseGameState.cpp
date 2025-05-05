#include "Gamestates/PauseGameState.h"
#include "Gamestate_Operators/Instanced/PauseMenuOperator.h"
#include "Gamestates/PlayMainGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include <SFML/Window/Keyboard.hpp>

void PauseGameState::Update(void) {
    // Check if player wants to unpause
    if(InputManager::IsKeyPressed(sf::Keyboard::Scan::Space))
        GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
}

void PauseGameState::Draw(sf::RenderWindow& window) {
    GetInstancedOperator<PauseMenuOperator>().Draw(window);
}