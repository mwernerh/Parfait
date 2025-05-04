#pragma once

#include <SFML/Graphics.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "EnemyManager.h"
#include "Literals.h"

/**
 * @author Isveydi Moreno
 * 
 */

class EnemyManager;

class LevelManager : public StaticInitializableGamestateOperator, public StaticUpdatableGamestateOperator, public StaticDrawableGamestateOperator {
    static u32 currentLevel;

    static std::vector<std::unique_ptr<EnemyManager>>& GetEnemyManagers(void);
    static void setupEnemies();

    public:
    static void Initialize(void);
    static void Update();


    static void Draw(sf::RenderWindow& window);
};