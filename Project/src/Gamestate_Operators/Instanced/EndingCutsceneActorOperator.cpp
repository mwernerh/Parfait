#include "Gamestate_Operators/Instanced/EndingCutsceneActorOperator.h"
#include "Framework_Managers/InputManager.h"
#include "SFML/Graphics/RenderWindow.hpp"

/**
 * @author Mar Werner Hernandez
 * 
 */

EndingCutsceneActorOperator::EndingCutsceneActorOperator() {
    cat.setOrigin(cat.getGlobalBounds().width / 2.0f, cat.getGlobalBounds().height / 2.0f);
    cat.setPosition(-200.0f, 600.0f);
    cat.scale(3.0f, 3.0f);
    cat.SetAnimation<"Walk">();
    cutsceneState = CutsceneState::WALK;
    catSpeed = 300.0f;
    stateTimer = 3.0f;
}

void EndingCutsceneActorOperator::Update(void) {
    cat.Update();

    switch(cutsceneState) {
        case CutsceneState::WALK:
            if(cat.getPosition().x < 600.0f) {
                cat.move({InputManager::GetDeltaTime() * catSpeed, 0.0f});
            }
            else {
                cutsceneState = CutsceneState::SIT;
                cat.SetAnimation<"Idle">();
            }
            break;
        case CutsceneState::SIT:
            if(catSpeed > 0.0f) {
                cat.move({InputManager::GetDeltaTime() * catSpeed, 0.0f});
                catSpeed /= 2.0f;
            }
            else {
                cutsceneState = CutsceneState::MEOW;
                cat.SetAnimation<"Attack">();
            }
            break;
        case CutsceneState::MEOW:
            if(stateTimer > 0) {
                stateTimer -= InputManager::GetDeltaTime();
            }
            else {
                cutsceneState = CutsceneState::END;
            }
            break;
        default:
            break;
    }
}

void EndingCutsceneActorOperator::Draw(sf::RenderWindow& window) {
    window.draw(cat);
}