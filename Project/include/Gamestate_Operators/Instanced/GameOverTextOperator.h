#pragma once
#include <SFML/Graphics/Text.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"

class GameOverTextOperator : public InstancedDrawableGamestateOperator {
    sf::Text gameoverText;

    public:
    GameOverTextOperator();
    void Draw(sf::RenderWindow& window) override;
};