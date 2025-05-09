#pragma once
#include "Gamestate_Operators/Instanced/EndingCutsceneActorOperator.h"
#include "Gamestates/BaseGameState.h"
#include <SFML/Graphics/Text.hpp>

class EndingCutsceneGameState : public InstancedGameState<EndingCutsceneGameState, EndingCutsceneActorOperator> {    
    sf::Sprite background;

    public:
    EndingCutsceneGameState();
    void Update(void);
    void Draw(sf::RenderWindow& draw);
};