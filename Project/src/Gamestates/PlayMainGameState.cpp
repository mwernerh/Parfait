#include "Gamestates/PlayMainGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include "Gamestates/PauseGameState.h"
#include "SFML/Window/Keyboard.hpp"

/**
 * @author Mar Werner Hernandez
 * 
 */

void PlayMainGameState::Initialize(void) {
    // Nothing additional needs to be done other than initializing the operators
    //   which is handled automatically by GamestateManager
}

void PlayMainGameState::Update(void) {
    // Check if player wants to pause
    if(InputManager::IsKeyPressed(sf::Keyboard::Scan::Space)) {
        GamestateManager::SwitchToInstancedGamestate<PauseGameState>();
    }
}

void PlayMainGameState::Draw(sf::RenderWindow& window) {
    // Draw order can be important, so GamestateManager doesn't handle it automatically

    // Update the camera to the player's position
    sf::Vector2f cameraPos = PlayerManager::GetPlayer().getPosition();
    sf::View cameraView = window.getView();
    cameraPos.y = cameraView.getCenter().y;

    cameraView.setCenter(cameraPos);
    window.setView(cameraView);

    // Set the audio listener to be at the player's position, so positional sounds are heard relative the player
    sf::Listener::setPosition(PlayerManager::GetPlayer().getPosition().x, PlayerManager::GetPlayer().getPosition().y, 0.0f);

    // Draw the main components of the scene in the proper order
    MapManager::Draw(window);       // Background
    LevelManager::Draw(window);     // Enemies
    PlayerManager::Draw(window);    // Player (& maybe health when implemented?)
    ScoreManager::Draw(window);     // ScoreText
}