#include "Gamestate_Operators/Instanced/TitleScreenActorOperator.h"
#include "Framework_Managers/InputManager.h"

void TitleScreenActorOperator::Update() {
    player.update(InputManager::GetDeltaTime());

    // Ensure the player doesn't move the camera
    if(std::abs(player.getPosition().x) < 200.0f) {
        player.setPosition({200.0f, player.getPosition().y});
    }
    else if(std::abs(player.getPosition().x) > 1080.0f) {
        player.setPosition({1080.0f, player.getPosition().y});
    }

    // npc.update() // Maybe not necessary

    // TODO: Add this when NPC is implemented
    /*
    if(player.getGlobalBounds().intersects(npc.getGlobalBounds()))
        npc.startDialogue();
    */
}

void TitleScreenActorOperator::Draw(sf::RenderWindow& window) {
    sf::View camera = window.getView();
    camera.setCenter(camera.getSize().x / 2.0f, camera.getSize().y / 2.0f);
    window.setView(camera);

    player.draw(window);
    // npc.draw(window);
}