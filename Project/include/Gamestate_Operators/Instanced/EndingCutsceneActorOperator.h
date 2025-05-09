#pragma once
#include "AnimatedSprite.h"
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "SFML/Graphics/RenderWindow.hpp"

class EndingCutsceneActorOperator : public InstancedDrawableGamestateOperator, public InstancedUpdatableGamestateOperator {
    AnimatedCatSprite2 cat;
    f32 stateTimer;
    f32 catSpeed;
    enum CutsceneState { WALK, SIT, MEOW, END } cutsceneState;


    // NPC npc; // TODO: Add NPC instance

    // u8 currentDialogue; // Maybe to loop through different dialogue options?

    public:
    EndingCutsceneActorOperator();
    void Update(void) override;
    void Draw(sf::RenderWindow&) override;
};