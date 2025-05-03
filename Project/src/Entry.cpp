#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "AnimationManager.h"
#include "HitboxManager.h"
#include "InputManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "AudioManager.h"
#include "LevelManager.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "TextureManager.h"

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

    // Initialize a player
    Player player("./assets/txr/animals/ct2/Walk.png", 20);
    HitboxManager::RegisterPlayer(&player);

    // Initialize various systems
    InputManager::Initialize();
    AudioManager::Initialize();
    TextureManager::Initialize();
    AnimationManager::Initialize();
    ScoreManager::Initialize();
    LevelManager::setupEnemies();
    
    // Set map to the park and play its appropriate BGM and ambience
    MapManager::ChangeMap(MapManager::Maps::PARK);
    AudioManager::StartMusic("./assets/aud/bgm_accordion.wav");
    AudioManager::StartMusic("./assets/aud/amb_pk.wav");

    AnimatedCatSprite1 bd;
    bd.setPosition({100, 100});
    bd.setScale({6.0f, 6.0f});
    bd.SetAnimation<"Walk">();

    while(window.isOpen()) {

        // Update delta time and prepare input manager for batch of new inputs
        InputManager::Update();
        
        // Update animation manager to see whether sprite animations should update in the current frame
        AnimationManager::Update();
    
        // Get batch of new inputs for InputManager
        HandleWindowEvents(window);

        // Update enemies of current level, and check if level should change
        LevelManager::Update(player);

        // Update Player
	    player.update(InputManager::GetDeltaTime());
        bd.setPosition(player.getPosition());

        // Update hitboxes
        HitboxManager::Update();

        // Update the camera
        sf::Vector2f cameraPos = player.getPosition();
        cameraPos.y = cameraView.getCenter().y;
        cameraView.setCenter(cameraPos);
        window.setView(cameraView);

        // Play audio
        AudioManager::Update();

        // Draw
        window.clear();
        MapManager::Draw(window);
        LevelManager::draw(window);
	    player.draw(window);

        // Update the animated sprite and draw it
        bd.Update();
        window.draw(bd);

        ScoreManager::Draw(window);
        window.display();

    }

    return 0;
}
