#include "Gamestates/PlayMainGameState.h"

void PlayMainGameState::Initialize(void) {
    // Nothing additional needs to be done other than initializing the operators
    //   which is handled automatically by GamestateManager
}

void PlayMainGameState::Update(void) {
    // Nothing additional needs to be done other than updating the operators
    //   which is handled automatically by GamestateManager
}

void PlayMainGameState::Draw(sf::RenderWindow& window) {
    // Draw order can be important, so GamestateManager doesn't handle it automatically

    // Update the camera
    sf::Vector2f cameraPos = PlayerManager::GetPlayer().getPosition();
    sf::View cameraView = window.getView();
    cameraPos.y = cameraView.getCenter().y;

    cameraView.setCenter(cameraPos);
    window.setView(cameraView);
    sf::Listener::setPosition(PlayerManager::GetPlayer().getPosition().x, PlayerManager::GetPlayer().getPosition().y, 0.0f);

    // Draw the main components of the scene in the proper order
    MapManager::Draw(window);       // Background
    LevelManager::Draw(window);     // Enemies
    PlayerManager::Draw(window);    // Player (& maybe health when implemented?)
    ScoreManager::Draw(window);     // ScoreText
}