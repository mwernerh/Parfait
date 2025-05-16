#include "Gamestate_Operators/Instanced/GameOverTextOperator.h"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/FontManager.h"
#include <SFML/Window/Keyboard.hpp>

/**
 * @author Mar Werner Hernandez
 * 
 */

GameOverTextOperator::GameOverTextOperator() : gameoverText("Game Over!", FontManager::GetFont("Outspace Fighter")) {
    gameoverText.setCharacterSize(32);
    gameoverText.setFillColor(sf::Color::White);
    gameoverText.setOrigin(gameoverText.getLocalBounds().width / 2.0f, gameoverText.getLocalBounds().height / 2.0f);
    gameoverText.setPosition({1280.0f / 2.0f, 720.0f / 2.0f});

    AudioManager::StartCameraSound("a_gm_ov");
}

void GameOverTextOperator::Draw(sf::RenderWindow& window) {
    sf::View view = window.getView();
    view.setCenter({view.getSize().x / 2.0f, view.getSize().y / 2.0f});
    window.setView(view);

    window.draw(gameoverText);
}