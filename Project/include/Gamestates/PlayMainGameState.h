#pragma once
#include "Gamestate_Operators/Static/MapManager.h"
#include "Gamestate_Operators/Static/HitboxManager.h"
#include "Gamestate_Operators/Static/LevelManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Gamestate_Operators/Static/ScoreManager.h"
#include "Gamestates/BaseGameState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Listener.hpp>

struct PlayMainGameState : public StaticGameState<PlayMainGameState, PlayerManager, HitboxManager, MapManager, LevelManager, ScoreManager> {
    static void Initialize(void) {
    }

    static void Update(void) {

    }

    static void Draw(sf::RenderWindow& window) {
        // Update the camera
        sf::Vector2f cameraPos = PlayerManager::GetPlayer().getPosition();
        sf::View cameraView = window.getView();
        cameraPos.y = cameraView.getCenter().y;

        cameraView.setCenter(cameraPos);
        window.setView(cameraView);
        sf::Listener::setPosition(PlayerManager::GetPlayer().getPosition().x, PlayerManager::GetPlayer().getPosition().y, 0.0f);

        // Draw the main components of the scene
        MapManager::Draw(window);
        LevelManager::Draw(window);
	    PlayerManager::Draw(window);
        ScoreManager::Draw(window);
    }
};