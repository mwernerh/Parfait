#pragma once
#include "Gamestate_Operators/Instanced/EndingCutsceneActorOperator.h"
#include "Gamestates/BaseGameState.h"
#include <SFML/Graphics/Text.hpp>

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Gamestate that handles drawing the countryside home background and the cutscene of the cat running and meowing. Switched to when player passes the third level
 * 
 */
class EndingCutsceneGameState : public InstancedGameState<EndingCutsceneGameState, EndingCutsceneActorOperator> {    
    sf::Sprite background;

    public:
    EndingCutsceneGameState();
    void Update(void);
    void Draw(sf::RenderWindow& draw);
};