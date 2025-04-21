#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "MapManager.h"
#include "ScoreManager.h"

int main(void) { 
    sf::RenderWindow window(sf::VideoMode{1280, 720}, "Team Parfait");
    window.setKeyRepeatEnabled(false);
    sf::View cameraView = window.getView();
    window.setView(cameraView);

    ScoreManager::Initialize();
    MapManager::ChangeMap(MapManager::Maps::CITY);

    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        // Check if the level should change
        //LevelManager::HandleCurrentLevel();

        // Update the view
        cameraView.setCenter(cameraView.getCenter());
        window.setView(cameraView);
        
        // Draw
        window.clear();
        MapManager::Draw(window, window.getView().getCenter(), 1.0f);
        ScoreManager::Draw(window);
        window.display();
    }

    return 0;
}