#pragma once

#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "Player.h"

class PlayerManager : public StaticInitializableGamestateOperator, public StaticUpdatableGamestateOperator, public StaticDrawableGamestateOperator {

    static Player& GetPlayerInternal(void);
    
    public:
    static const Player& GetPlayer(void);
    static void Initialize(void);
    static void Update(void);
    static void Draw(sf::RenderWindow& window);
};