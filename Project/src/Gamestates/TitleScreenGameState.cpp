#include "Gamestates/TitleScreenGameState.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/FontManager.h"
#include "Framework_Managers/SaveManager.h"
#include "Framework_Managers/AudioManager.h"
#include "Gamestate_Operators/Instanced/TitleScreenActorOperator.h"
#include "Gamestates/PlayMainGameState.h"

/**
 * @author Mar Werner Hernandez
 * @author Izzy Carlson
 * 
 */

TitleScreenGameState::TitleScreenGameState() : InstancedGameState(), startText("Press spacebar to start!", FontManager::GetFont("Outspace Fighter")) {
    // set up background
    background.setTexture(TextureManager::GetTexture("bgs/s_ct/1.png"));
    background.setOrigin(0.f, 0.f);
    background.setPosition(-50.f, -150.f);
    
    titleText.setFont(FontManager::GetFont("Outspace Fighter"));
    titleText.setString("Homesick");
    titleText.setCharacterSize(54);
    titleText.setOrigin(titleText.getLocalBounds().width / 2.0f, titleText.getLocalBounds().height / 2.0f);
    titleText.setPosition({1280.0f / 2.f, (720.0f / 2.0f) - (720.f / 3.0f)});
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineThickness(2.f);
    titleText.setOutlineColor(sf::Color::Black);

    startText.setOrigin(startText.getLocalBounds().width / 2.0f, startText.getLocalBounds().height / 2.0f);
    startText.setPosition({1280.0f / 2.f, (720.0f / 2.f) - (720.f / 4.0f)});
    startText.setFillColor(sf::Color::White);
    startText.setOutlineThickness(2.f);
    startText.setOutlineColor(sf::Color::Black);
}

void TitleScreenGameState::Update() {
    if(InputManager::IsKeyPressed(sf::Keyboard::Scancode::Space)) {
        GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
	AudioManager::ClearCameraSounds();
        SaveManager::ReadFromFile();
    }
}

void TitleScreenGameState::Draw(sf::RenderWindow& window) {
    window.draw(background);
    GetInstancedOperator<TitleScreenActorOperator>().Draw(window);
    window.draw(titleText);
    window.draw(startText);
}
