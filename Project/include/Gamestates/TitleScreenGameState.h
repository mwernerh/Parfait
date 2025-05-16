#pragma once
#include "Gamestate_Operators/Instanced/TitleScreenActorOperator.h"
#include "Gamestates/BaseGameState.h"
#include <SFML/Graphics/Text.hpp>

/**
 * @author Mar Werner Hernandez
 * @author Izzy Carlson
 * 
 */

/**
 * @brief First gamestate of the game that acts as a tutorial
 * 
 */
class TitleScreenGameState : public InstancedGameState<TitleScreenGameState, TitleScreenActorOperator> {
    sf::Sprite background;
    sf::Text titleText;
    sf::Text startText;
    
    public:
    TitleScreenGameState();
    void Update(void);
    void Draw(sf::RenderWindow& draw);
};
