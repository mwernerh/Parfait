#include "Gamestates/TitleScreenGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/FontManager.h"
#include "Gamestate_Operators/Instanced/TitleScreenActorOperator.h"
#include "Gamestates/PlayMainGameState.h"

TitleScreenGameState::TitleScreenGameState() : InstancedGameState(), startText("Press spacebar to start!", FontManager::GetFont("Outspace Fighter")) {
    // set up background
    background.setTexture(TextureManager::GetTexture("bgs/s_ct/1.png"));
    background.setOrigin(0.f, 0.f);
    background.setPosition(-50.f, -150.f);
	
    startText.setOrigin(startText.getLocalBounds().width / 2.0f, startText.getLocalBounds().height / 2.0f);
    startText.setPosition({1280.0f / 2.f, (720.0f / 2.f) - (720.f / 4.f)});
    startText.setFillColor(sf::Color::White);
    startText.setOutlineThickness(2.f);
    startText.setOutlineColor(sf::Color::Black);
}

void TitleScreenGameState::Update() {
    if(InputManager::IsKeyPressed(sf::Keyboard::Scancode::Space))
        GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
}

void TitleScreenGameState::Draw(sf::RenderWindow& window) {
    window.draw(background);
    GetInstancedOperator<TitleScreenActorOperator>().Draw(window);
    window.draw(startText);
}
