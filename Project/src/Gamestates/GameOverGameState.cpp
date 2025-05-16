#include "Gamestates/GameOverGameState.h"
#include "Gamestate_Operators/Instanced/GameOverTextOperator.h"
#include "Gamestate_Operators/Static/LevelManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Gamestates/PlayMainGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/SaveManager.h"
#include <SFML/Window/Keyboard.hpp>

/**
 * @author Mar Werner Hernandez
 * 
 */

void GameOverGameState::Update(void) {
    // Player wants to start from latest save
    if(InputManager::IsKeyPressed(sf::Keyboard::Scan::Space)) {
        GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
        SaveManager::ReadFromFile();

        // If player health is low, give them more so that they have a better chance of progressing through the game
        if(PlayerManager::GetPlayer().getHealth() < 50) {
            PlayerManager::SetPlayerHealth(50);
        }
    }
}

void GameOverGameState::Draw(sf::RenderWindow& window) {
    GetInstancedOperator<GameOverTextOperator>().Draw(window);
}