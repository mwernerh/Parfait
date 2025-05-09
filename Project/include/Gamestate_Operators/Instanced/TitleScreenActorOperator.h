#pragma once
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Player.h"

class TitleScreenActorOperator : public InstancedDrawableGamestateOperator, public InstancedUpdatableGamestateOperator {
    Player player;
    // NPC npc; // TODO: Add NPC instance

    // u8 currentDialogue; // Maybe to loop through different dialogue options?

    public:
    void Update(void) override;
    void Draw(sf::RenderWindow&) override;
};