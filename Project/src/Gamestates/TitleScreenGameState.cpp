#include "Gamestates/TitleScreenGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/FontManager.h"
#include "Gamestate_Operators/Instanced/TitleScreenActorOperator.h"
#include "Gamestates/PlayMainGameState.h"

TitleScreenGameState::TitleScreenGameState() : InstancedGameState(), startText("Press spacebar to start!", FontManager::GetFont("Outspace Fighter")) {
    startText.setOrigin(startText.getLocalBounds().width / 2.0f, startText.getLocalBounds().height / 2.0f);
    startText.setPosition({1280.0f / 2.0f, 720.0f / 2.0f});
    startText.setFillColor(sf::Color::White);
}

void TitleScreenGameState::Update() {
    if(InputManager::IsKeyPressed(sf::Keyboard::Scancode::Space))
        GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
}

void TitleScreenGameState::Draw(sf::RenderWindow& window) {
    GetInstancedOperator<TitleScreenActorOperator>().Draw(window);
    window.draw(startText);
}