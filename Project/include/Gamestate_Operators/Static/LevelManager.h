#pragma once

#include <SFML/Graphics.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "EnemyManager.h"
#include "DogEnemy.h"
#include "RatEnemy.h"
#include "BirdEnemy.h"
#include "Literals.h"

/**
 * @author Isveydi Moreno
 * 
 */


class LevelManager : public StaticInitializableGamestateOperator<LevelManager>, public StaticUpdatableGamestateOperator<LevelManager>, public StaticDrawableGamestateOperator<LevelManager> {
    static u32 currentLevel;

    static std::vector<std::unique_ptr<EnemyManager<Enemy>>>& GetEnemyManagers(void);
    static void setupEnemies();

    public:
    static void Initialize(void);
    static void Update(void);

    static const u32& GetCurrentLevel(void); 
    static void SetCurrentLevel(const u32 level);

    static void Draw(sf::RenderWindow& window);
};