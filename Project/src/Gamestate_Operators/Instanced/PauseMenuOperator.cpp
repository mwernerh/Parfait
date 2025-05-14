#include "Gamestate_Operators/Instanced/PauseMenuOperator.h"
#include "Framework_Managers/FontManager.h"
#include <SFML/Window/Keyboard.hpp>

PauseMenuOperator::PauseMenuOperator() : pauseText("Press Spacebar to unpause\nPress K to save", FontManager::GetFont("Outspace Fighter")) {
    pauseText.setCharacterSize(32);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOrigin(pauseText.getLocalBounds().width / 2.0f, pauseText.getLocalBounds().height / 2.0f);
    pauseText.setPosition({1280.0f / 2.0f, 720.0f / 2.0f});
}

void PauseMenuOperator::Draw(sf::RenderWindow& window) {
    sf::View view = window.getView();
    view.setCenter({view.getSize().x / 2.0f, view.getSize().y / 2.0f});
    window.setView(view);

    window.draw(pauseText);
}