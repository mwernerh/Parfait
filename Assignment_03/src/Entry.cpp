#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "HitboxManager.h"
#include "InputManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "AudioManager.h"

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

    ScoreManager::Initialize();
    AudioManager::Initialize();
    InputManager::Initialize();

    MapManager::ChangeMap(MapManager::Maps::PARK);
    AudioManager::StartMusic("./assets/aud/bgm_accordion.wav");
    AudioManager::StartMusic("./assets/aud/amb_pk.wav");

    while(window.isOpen()) {

        // Update delta time and prepare input manager for batch of new inputs
        InputManager::Update();
        
        HandleWindowEvents(window);

        if(InputManager::IsKeyPressed(sf::Keyboard::Scancode::X)) {
            AudioManager::StartCameraSound("./assets/aud/e_dg_atk.wav");
            ScoreManager::AddScore(100);
        }

        // Check if the level should change
        //LevelManager::Update();

        // Update hitboxes
        HitboxManager::Update();

        // Update the camera
        cameraView.setCenter(cameraView.getCenter()); // TODO: Set camera center's horizontal position to be at the player's horizontal position
        window.setView(cameraView);

        // Play audio
        AudioManager::Update();
        
        // Draw
        window.clear();
        MapManager::Draw(window, window.getView().getCenter(), InputManager::GetDeltaTime() * 150.0f);
        ScoreManager::Draw(window);
        window.display();

    }

    return 0;
}