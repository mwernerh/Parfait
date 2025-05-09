#pragma once
#include "Gamestate_Operators/Instanced/TitleScreenActorOperator.h"
#include "Gamestates/BaseGameState.h"
#include <SFML/Graphics/Text.hpp>

class TitleScreenGameState : public InstancedGameState<TitleScreenGameState, TitleScreenActorOperator> {
    sf::Text startText;
    
    public:
    TitleScreenGameState();
    void Update(void);
    void Draw(sf::RenderWindow& draw);
};