#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Framework_Managers/AnimationManager.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/TextureManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/FontManager.h"
#include "Gamestates/PlayMainGameState.h"

void HandleWindowEvents(sf::WindowBase& window) {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
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
    sf::RenderWindow window(sf::VideoMode{1280, 720}, "Team Parfait");
    window.setKeyRepeatEnabled(false);
    sf::View cameraView = window.getView();
    window.setView(cameraView);

    // Initialize various framework systems used by the gamestates
    InputManager::Initialize();
    AudioManager::Initialize();
    TextureManager::Initialize();
    AnimationManager::Initialize();
    FontManager::Initialize();

    /*
    InputManager::Initialize();
    AudioManager::Initialize();
    TextureManager::Initialize();
    AnimationManager::Initialize();
    PlayerManager::Initialize();
    ScoreManager::Initialize();
    MapManager::Initialize();
    LevelManager::Initialize();
    */

    GamestateManager::SwitchToStaticGamestate<PlayMainGameState>();
    GamestateManager::InitializeOperators();

    /*
    AnimatedCatSprite1 bd;
    bd.setPosition({100, 100});
    bd.setScale({6.0f, 6.0f});
    bd.SetAnimation<"Walk">();
    */

    while(window.isOpen()) {

        // Update delta time and prepare input manager for batch of new inputs
        InputManager::Update();
        
        // Update animation manager to see whether sprite animations should update in the current frame
        AnimationManager::Update();
    
        // Get batch of new inputs for InputManager
        HandleWindowEvents(window);

        /*
        // Update enemies of current level, and check if level should change
        LevelManager::Update();

        // Update Player
	    PlayerManager::Update();
        //bd.setPosition(PlayerManager::GetPlayer().getPosition());

        // Update hitboxes
        HitboxManager::Update();

        */

        GamestateManager::UpdateOperators();

        // Play audio
        AudioManager::Update();

        // Draw
        window.clear();

        GamestateManager::DrawOperators(window);

        /*
        MapManager::Draw(window);
        LevelManager::Draw(window);
	    PlayerManager::Draw(window);

        // Update the animated sprite and draw it
        bd.Update();
        window.draw(bd);

        ScoreManager::Draw(window);
        */

        window.display();

    }

    return 0;
}
