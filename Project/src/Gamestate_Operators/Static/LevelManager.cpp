#include "Gamestate_Operators/Static/LevelManager.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/SaveManager.h"
#include "Gamestate_Operators/Static/MapManager.h"
#include "Gamestate_Operators/Static/ScoreManager.h"
#include "EnemyManager.h"
#include "DogEnemy.h"
#include "RatEnemy.h"
#include "BirdEnemy.h"
#include "Gamestates/EndingCutsceneGameState.h"

/**
 * @author Isveydi Moreno
 * @author Mar Werner Hernandez
 */

u32 LevelManager::currentLevel = 0; // Current level of the game

std::vector<std::unique_ptr<EnemyManager<Enemy>>>& LevelManager::GetEnemyManagers(void) { // Get the enemy managers
    static std::vector<std::unique_ptr<EnemyManager<Enemy>>> em; // Create a static vector of unique pointers to EnemyManager
    return em;
}

[[gnu::hot]]
void LevelManager::Update() {
    std::vector<std::unique_ptr<EnemyManager<Enemy>>>& enemyManagers = GetEnemyManagers();
    for (auto& enemyManager : enemyManagers) {
        enemyManager->update();
    }

    [[unlikely]]
    if(ScoreManager::GetScore() >= (1000 * currentLevel) + 1000) {
        currentLevel += 1;

        if(currentLevel != 3)
            SaveManager::WriteToFile(SaveManager::SaveType::AUTO);

        // Reached last level and passed it
        if(currentLevel == 3) {
            GamestateManager::SwitchToInstancedGamestate<EndingCutsceneGameState>();
            return;
        }

        // MAR_TODO: Implement sound for notifying level change
        MapManager::ChangeMap(static_cast<MapManager::Maps>(currentLevel % static_cast<decltype(currentLevel)>(MapManager::Maps::NUM_MAPS)));
        setupEnemies();
    }
}

void LevelManager::Initialize() {
    setupEnemies();
}

// LEVEL MANAGER SETUP ENEMIES
// This function sets up the enemies for the current level
void LevelManager::setupEnemies() {
    std::vector<std::unique_ptr<EnemyManager<Enemy>>>& enemyManagers = GetEnemyManagers();

    enemyManagers.clear(); // Clear the existing enemy managers

    // Create new enemy managers for the current level
    std::unique_ptr<EnemyManager<Enemy>> manager1 = std::bit_cast<std::unique_ptr<EnemyManager<Enemy>>>(std::make_unique<EnemyManager<RatEnemy>>());
    std::unique_ptr<EnemyManager<Enemy>> manager2 = std::bit_cast<std::unique_ptr<EnemyManager<Enemy>>>(std::make_unique<EnemyManager<BirdEnemy>>());
    std::unique_ptr<EnemyManager<Enemy>> manager3 = std::bit_cast<std::unique_ptr<EnemyManager<Enemy>>>(std::make_unique<EnemyManager<DogEnemy>>());
    std::unique_ptr<EnemyManager<Enemy>> manager4 = std::bit_cast<std::unique_ptr<EnemyManager<Enemy>>>(std::make_unique<EnemyManager<DogEnemy>>());


    switch (currentLevel){
        case 0: //set up rat for level 1
            manager1->configure("./assets/txr/animals/rt1/Walk.png", 5, 3, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager1));
            //manager1->update();
            manager2->configure("./assets/txr/animals/bd1/Walk.png", 5, 1, 2.0f, .40f);
            enemyManagers.push_back(std::move(manager2));
            //manager2->update();
            break;

        case 1: //set up bird nd dog 2 for level 2
            manager3->configure("./assets/txr/animals/dg2/Walk.png", 5, 1, 2.0f, .40f);
            enemyManagers.push_back(std::move(manager3));
            //manager3->update();
            manager2->configure("./assets/txr/animals/bd1/Walk.png", 5, 1, 2.0f, .40f);
            enemyManagers.push_back(std::move(manager2));
            //manager2->update();
            

        case 2: //set up dog 1 and bird for level 3
            manager4->configure("./assets/txr/animals/dg1/Walk.png", 5, 3, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager4));
            //manager4->update();
            manager2->configure("./assets/txr/animals/bd1/Walk.png", 5, 1, 2.0f, .40f);
            //enemyManagers.push_back(std::move(manager2));
            enemyManagers.push_back(std::move(manager2));
            //manager2->update();
            break;
        default: // setup dog 1 and 2 enemy as default
            manager4->configure("./assets/txr/animals/dg1/Walk.png", 5, 3, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager4));
            //enemyManagers.push_back(std::move(manager4));
            //manager4->update();

            manager3->configure("./assets/txr/animals/dg2/Walk.png", 5, 1, 2.0f, .40f);
            enemyManagers.push_back(std::move(manager3));
            //enemyManagers.push_back(std::move(manager3));
            //manager3->update();
            

            break;
    };

}

const u32& LevelManager::GetCurrentLevel(void) {
    return currentLevel;
}

void LevelManager::SetCurrentLevel(const u32 level) {
    currentLevel = level;
    MapManager::ChangeMap(static_cast<MapManager::Maps>(currentLevel % static_cast<decltype(currentLevel)>(MapManager::Maps::NUM_MAPS)));
    setupEnemies();
}

//LEVEL MANAGER DRAW
// This function draws the enemies to the window
void LevelManager::Draw(sf::RenderWindow& window){
    std::vector<std::unique_ptr<EnemyManager<Enemy>>>& enemyManagers = GetEnemyManagers(); // Get the enemy managers
    for (auto& EnemyManager: enemyManagers){ // Loop through each enemy manager
        EnemyManager->draw(window); // Draw the enemie to the window
        EnemyManager->drawHealthBar(window); // Draw the health bar for each enemy
    }
}
