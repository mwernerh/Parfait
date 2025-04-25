#include "LevelManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "EnemyManager.h"


u32 LevelManager::currentLevel = 0;
std::vector<std::unique_ptr<EnemyManager>> LevelManager::enemyManagers ={};

[[gnu::hot]]
void LevelManager::Update(Player& player) {
    for (auto& enemyManager : enemyManagers) {
        enemyManager->update(InputManager::GetDeltaTime(), player);
    }

    [[unlikely]]
    if(ScoreManager::GetScore() >= (1000 * currentLevel) + 100) {
        currentLevel += 1;
        MapManager::ChangeMap(static_cast<MapManager::Maps>(currentLevel % static_cast<decltype(currentLevel)>(MapManager::Maps::NUM_MAPS)));
        setupEnemies(currentLevel);
    }
}

void LevelManager::setupEnemies(int currentLevel) {

    //look at bee game to set the position from side to side
    //Have the enemies spawn from the left side of the screen and move to the right
    enemyManagers.clear();

    switch (currentLevel){
        case 0:{
            // setup dog enemy for the current level
            auto manager = std::make_unique<EnemyManager>();
            manager->configure("./assets/txr/animals/dg1/Walk.png", 20, 100, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager));
        }
    };

}

void LevelManager::draw(sf::RenderWindow& window){
    for (auto& EnemyManager: enemyManagers){
        EnemyManager->draw(window);
    }
}