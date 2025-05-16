#include "Gamestates/PauseGameState.h"
#include "Framework_Managers/SaveManager.h"
#include "Gamestate_Operators/Instanced/PauseMenuOperator.h"
#include "Gamestates/PlayMainGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include <SFML/Window/Keyboard.hpp>

/**
 * @author Mar Werner Hernandez
 * 
 */

void PauseGameState::Update(void) {
    // Check if player wants to unpause or save
    if(InputManager::IsKeyPressed(sf::Keyboard::Scan::Space))
        GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
    else if(InputManager::IsKeyPressed(sf::Keyboard::Scan::K)) {
        SaveManager::WriteToFile(SaveManager::SaveType::MANUAL);
    }
}

void PauseGameState::Draw(sf::RenderWindow& window) {
    GetInstancedOperator<PauseMenuOperator>().Draw(window);
}