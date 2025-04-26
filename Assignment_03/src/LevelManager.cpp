#include "LevelManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "EnemyManager.h"

/**
 * @author Isveydi Moreno
 * @author Mar Werner Hernandez
 */

u32 LevelManager::currentLevel = 0; // Current level of the game

std::vector<std::unique_ptr<EnemyManager>>& LevelManager::GetEnemyManagers(void) { // Get the enemy managers
    static std::vector<std::unique_ptr<EnemyManager>> em; // Create a static vector of unique pointers to EnemyManager
    em.reserve(2); // Reserve space for 2 enemy managers
    return em;
}

[[gnu::hot]]
void LevelManager::Update(Player& player) {
    std::vector<std::unique_ptr<EnemyManager>>& enemyManagers = GetEnemyManagers();
    for (auto& enemyManager : enemyManagers) {
        enemyManager->update(InputManager::GetDeltaTime(), player);
    }

    [[unlikely]]
    if(ScoreManager::GetScore() >= (1000 * currentLevel) + 1000) {
        if(currentLevel <= 0) {
            currentLevel += 1;
            setupEnemies();
        }
        //MapManager::ChangeMap(static_cast<MapManager::Maps>(currentLevel % static_cast<decltype(currentLevel)>(MapManager::Maps::NUM_MAPS)));
        
    }
}

// LEVEL MANAGER SETUP ENEMIES
// This function sets up the enemies for the current level
void LevelManager::setupEnemies() {
    std::vector<std::unique_ptr<EnemyManager>>& enemyManagers = GetEnemyManagers();

    enemyManagers.clear(); // Clear the existing enemy managers

    // Create new enemy managers for the current level
    auto manager1 = std::make_unique<EnemyManager>();
    auto manager2 = std::make_unique<EnemyManager>();

    switch (currentLevel){
        case 0:
            // setup dog 1 enemy for the current level
            manager1->configure("./assets/txr/animals/dg1/Walk.png", 5, 3, 2.0f, .20f); //dog 1 texture
            enemyManagers.push_back(std::move(manager1));
            break;
        default: // setup dog 1 and 2 enemy as default
            manager1->configure("./assets/txr/animals/dg1/Walk.png", 5, 3, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager1));

            manager2->configure("./assets/txr/animals/dg2/Walk.png", 5, 1, 2.0f, .40f);
            enemyManagers.push_back(std::move(manager2));
            break;
    };

}

//LEVEL MANAGER DRAW
// This function draws the enemies to the window
void LevelManager::draw(sf::RenderWindow& window){
    std::vector<std::unique_ptr<EnemyManager>>& enemyManagers = GetEnemyManagers(); // Get the enemy managers
    for (auto& EnemyManager: enemyManagers){ // Loop through each enemy manager
        EnemyManager->draw(window); // Draw the enemie to the window
    }
}
