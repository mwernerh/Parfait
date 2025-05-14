#include "Gamestate_Operators/Static/LevelManager.h"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/GamestateManager.h"
#include "Framework_Managers/SaveManager.h"
#include "Gamestate_Operators/Static/MapManager.h"
#include "Gamestate_Operators/Static/ScoreManager.h"
#include "EnemyManager.h"
#include "DogEnemy.h"
#include "DogEnemy2.h"
#include "RatEnemy.h"
#include "BirdEnemy.h"
#include "Gamestates/EndingCutsceneGameState.h"

/**
 * @author Isveydi Moreno
 * @author Mar Werner Hernandez
 */

u32 LevelManager::currentLevel = 0; // Current level of the game

LevelManager::EnemyManagers& LevelManager::GetEnemyManagers(void) { // Get the enemy managers
    static EnemyManagers em; // Create a static vector of unique pointers to EnemyManager
    return em;
}

[[gnu::hot]]
void LevelManager::Update() {
    EnemyManagers& enemyManagers = GetEnemyManagers();
    if(enemyManagers.birdManager)
        enemyManagers.birdManager->update();
    if(enemyManagers.ratManager)
        enemyManagers.ratManager->update();
    if(enemyManagers.dogManager1)
        enemyManagers.dogManager1->update();
    if(enemyManagers.dogManager2)
        enemyManagers.dogManager2->update();

    [[unlikely]]
    if(ScoreManager::GetScore() >= (1000 * currentLevel) + 1000) {
        currentLevel += 1;

        if(currentLevel != 3)
            SaveManager::WriteToFile(SaveManager::SaveType::AUTO);

        AudioManager::StartCameraSound("a_lv_up");

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
    EnemyManagers& enemyManagers = GetEnemyManagers();

    // Clear the existing enemy managers
    enemyManagers.birdManager.reset();
    enemyManagers.dogManager1.reset();
    enemyManagers.dogManager2.reset();
    enemyManagers.ratManager.reset();


    switch (currentLevel){
        case 0: //set up rat for level 1
            enemyManagers.birdManager = std::make_unique<EnemyManager<BirdEnemy>>();
            enemyManagers.ratManager = std::make_unique<EnemyManager<RatEnemy>>();
            enemyManagers.ratManager->configure("./assets/txr/animals/rt1/Walk.png", 2, 3, 2.0f, .20f);
            enemyManagers.birdManager->configure("./assets/txr/animals/bd1/Walk.png", 2, 1, 2.0f, .40f);
            break;

        case 1: //set up bird nd dog 2 for level 2
            enemyManagers.birdManager = std::make_unique<EnemyManager<BirdEnemy>>();
            enemyManagers.dogManager2 = std::make_unique<EnemyManager<DogEnemy2>>();
            enemyManagers.dogManager2->configure("./assets/txr/animals/dg2/Walk.png", 2, 1, 2.0f, .40f);
            enemyManagers.birdManager->configure("./assets/txr/animals/bd1/Walk.png", 2, 1, 2.0f, .40f);
            break;
        case 2: //set up dog 1 and bird for level 3
            enemyManagers.birdManager = std::make_unique<EnemyManager<BirdEnemy>>();
            enemyManagers.dogManager1 = std::make_unique<EnemyManager<DogEnemy>>();
            enemyManagers.dogManager1->configure("./assets/txr/animals/dg1/Walk.png", 2, 3, 2.0f, .20f);
            enemyManagers.birdManager->configure("./assets/txr/animals/bd1/Walk.png", 2, 1, 2.0f, .40f);
            break;
        default: // setup dog 1 and 2 enemy as default
            enemyManagers.dogManager1 = std::make_unique<EnemyManager<DogEnemy>>();
            enemyManagers.dogManager2 = std::make_unique<EnemyManager<DogEnemy2>>();
            enemyManagers.dogManager1->configure("./assets/txr/animals/dg1/Walk.png", 2, 3, 2.0f, .20f);
            enemyManagers.dogManager2->configure("./assets/txr/animals/dg2/Walk.png", 2, 1, 2.0f, .40f);
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
    EnemyManagers& enemyManagers = GetEnemyManagers(); // Get the enemy managers

    if(enemyManagers.birdManager) {
        enemyManagers.birdManager->draw(window);
        enemyManagers.birdManager->drawHealthBar(window);
    }

    if(enemyManagers.ratManager) {
        enemyManagers.ratManager->draw(window);
        enemyManagers.ratManager->drawHealthBar(window);
    }

    if(enemyManagers.dogManager1) {
        enemyManagers.dogManager1->draw(window);
        enemyManagers.dogManager1->drawHealthBar(window);
    }

    if(enemyManagers.dogManager2) {
        enemyManagers.dogManager2->draw(window);
        enemyManagers.dogManager2->drawHealthBar(window);
    }
}