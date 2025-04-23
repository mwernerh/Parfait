#include "LevelManager.h"
#include "MapManager.h"
#include "ScoreManager.h"

u32 LevelManager::currentLevel = 0;

[[gnu::hot]]
void LevelManager::Update() {
    [[unlikely]]
    if(ScoreManager::GetScore() >= (1000 * currentLevel) + 100) {
        currentLevel += 1;
        MapManager::ChangeMap(static_cast<MapManager::Maps>(currentLevel % static_cast<decltype(currentLevel)>(MapManager::Maps::NUM_MAPS)));
    }
}