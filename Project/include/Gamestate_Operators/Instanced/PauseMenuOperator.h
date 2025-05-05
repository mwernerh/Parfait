#pragma once
#include <SFML/Graphics/Text.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"

class PauseMenuOperator : public InstancedDrawableGamestateOperator {
    sf::Text pauseText;

    public:
    PauseMenuOperator();
    void Draw(sf::RenderWindow& window) override;
};