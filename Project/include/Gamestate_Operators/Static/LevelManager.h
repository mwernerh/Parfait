#pragma once

#include <SFML/Graphics.hpp>
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "EnemyManager.h"
#include "DogEnemy.h"
#include "DogEnemy2.h"
#include "RatEnemy.h"
#include "BirdEnemy.h"
#include "Literals.h"

/**
 * @author Isveydi Moreno
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Interface to switch between levels, which entails changing map and spawning the corresponding actors for each level
 * 
 */
class LevelManager : public StaticInitializableGamestateOperator<LevelManager>, public StaticUpdatableGamestateOperator<LevelManager>, public StaticDrawableGamestateOperator<LevelManager> {
    static u32 currentLevel;

    struct EnemyManagers {
        std::unique_ptr<EnemyManager<RatEnemy>> ratManager;
        std::unique_ptr<EnemyManager<BirdEnemy>> birdManager;
        std::unique_ptr<EnemyManager<DogEnemy>> dogManager1;
        std::unique_ptr<EnemyManager<DogEnemy2>> dogManager2;
    };

    /**
     * @brief Get the EnemyManagers singleton
     * 
     * @return EnemyManagers& 
     */
    static EnemyManagers& GetEnemyManagers(void);

    /**
     * @brief Setup the enemies for the current level
     * 
     */
    static void setupEnemies();

    public:
    /**
     * @brief Sets up the enemies for the default level
     * 
     */
    static void Initialize(void);

    /**
     * @brief Updates the enemies in the current level
     * 
     */
    static void Update(void);

    /**
     * @brief Obtains the current level
     * 
     * @return const u32& 
     */
    static const u32& GetCurrentLevel(void);
    
    /**
     * @brief Sets the current level
     * 
     * @param level 
     */
    static void SetCurrentLevel(const u32 level);

    /**
     * @brief Draws the enemies
     * 
     * @param window 
     */
    static void Draw(sf::RenderWindow& window);
};