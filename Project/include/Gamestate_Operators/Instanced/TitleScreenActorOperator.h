#pragma once
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Player.h"
#include "NPC.h"

/**
 * @author Mar Werner Hernandez
 * @author Izzy Carlson
 * 
 */

/**
 * @brief Gamestate operator that handles updating and drawing the ineractables (actors -- namely NPC and player)
 * 
 */
class TitleScreenActorOperator : public InstancedDrawableGamestateOperator, public InstancedUpdatableGamestateOperator {
    Player player;
    NPC npc; // TODO: Add NPC instance

    // u8 currentDialogue; // Maybe to loop through different dialogue options?

    public:
    void Update(void) override;
    void Draw(sf::RenderWindow&) override;
};
