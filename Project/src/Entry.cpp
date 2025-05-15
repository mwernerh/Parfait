#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Framework_Managers/AnimationManager.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/SaveManager.h"
#include "Framework_Managers/TextureManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/FontManager.h"
#include "Gamestate_Operators/Static/LevelManager.h"
#include "Gamestates/TitleScreenGameState.h"

void HandleWindowEvents(sf::WindowBase& window) {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                if(LevelManager::GetCurrentLevel() != 3)
                    SaveManager::WriteToFile(SaveManager::SaveType::AUTO);
                window.close();
                break;
            case sf::Event::KeyPressed:
                InputManager::RegisterKeyPress(event.key.scancode);
                break;
            case sf::Event::KeyReleased:
                InputManager::RegisterKeyRelease(event.key.scancode);
                break;
            default:
                break;
        }
    }
}

int main(void) { 
    sf::RenderWindow window(sf::VideoMode{1280, 720}, "Homesick");
    window.setKeyRepeatEnabled(false);
    sf::View cameraView = window.getView();
    window.setView(cameraView);

    // Initialize various framework systems used by the gamestates
    InputManager::Initialize();
    AudioManager::Initialize();
    TextureManager::Initialize();
    AnimationManager::Initialize();
    FontManager::Initialize();

    GamestateManager::SwitchToInstancedGamestate<TitleScreenGameState>();

    while(window.isOpen()) {

        // Update delta time and prepare input manager for batch of new inputs
        InputManager::Update();
        
        // Update animation manager to see whether sprite animations should update in the current frame
        AnimationManager::Update();
    
        // Get batch of new inputs for InputManager
        HandleWindowEvents(window);

        // Update current gamestate
        GamestateManager::UpdateOperators();

        // Play audio
        AudioManager::Update();

        // Draw
        window.clear();

        // Draw current gamestate
        GamestateManager::DrawOperators(window);

        window.display();

    }

    return 0;
}
