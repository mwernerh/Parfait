#include "Gamestates/EndingCutsceneGameState.h"
#include "Gamestate_Operators/Instanced/EndingCutsceneActorOperator.h"

EndingCutsceneGameState::EndingCutsceneGameState() {
    background.setTexture(TextureManager::GetTexture("bgs/s_hm/1.png"));
    background.setOrigin(0.0f, 0.0f);
    background.setPosition(0.0f, 0.0f);
    background.setScale(1280.0f / background.getTexture()->getSize().x, 720.0f / background.getTexture()->getSize().y);
}

void EndingCutsceneGameState::Update() {
    // No-op for now -- switch back to title screen once saving properly implemented
}

void EndingCutsceneGameState::Draw(sf::RenderWindow& window) {
    sf::View camera = window.getView();
    camera.setCenter(camera.getSize().x / 2.0f, camera.getSize().y / 2.0f);
    window.setView(camera);

    window.draw(background);
    GetInstancedOperator<EndingCutsceneActorOperator>().Draw(window);
}