#include "LevelManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "EnemyManager.h"


u32 LevelManager::currentLevel = 0;

std::vector<std::unique_ptr<EnemyManager>>& LevelManager::GetEnemyManagers(void) {
    static std::vector<std::unique_ptr<EnemyManager>> em;
    em.reserve(2);
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

void LevelManager::setupEnemies() {
    std::vector<std::unique_ptr<EnemyManager>>& enemyManagers = GetEnemyManagers();
    //look at bee game to set the position from side to side
    //Have the enemies spawn from the left side of the screen and move to the right
    enemyManagers.clear();

    auto manager1 = std::make_unique<EnemyManager>();
    auto manager2 = std::make_unique<EnemyManager>();

    switch (currentLevel){
        case 0:
            // setup dog enemy for the current level
            manager1->configure("./assets/txr/animals/dg1/Walk.png", 5, 3, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager1));
            break;
        default:
            manager1->configure("./assets/txr/animals/dg1/Walk.png", 5, 3, 2.0f, .20f);
            enemyManagers.push_back(std::move(manager1));

            manager2->configure("./assets/txr/animals/dg2/Walk.png", 5, 1, 2.0f, .40f);
            enemyManagers.push_back(std::move(manager2));
            break;
    };

}

void LevelManager::draw(sf::RenderWindow& window){
    std::vector<std::unique_ptr<EnemyManager>>& enemyManagers = GetEnemyManagers();
    for (auto& EnemyManager: enemyManagers){
        EnemyManager->draw(window);
    }
}
