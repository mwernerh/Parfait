//#pragma once

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#include <SFML/Graphics.hpp>
#include "MapManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Literals.h"

class EnemyManager;

class LevelManager {
    static u32 currentLevel;
    static std::vector<std::unique_ptr<EnemyManager>> enemyManagers;

    public:
    LevelManager();
    static void Update(Player& player);

    static void setupEnemies(int currentLevel);

    static void draw(sf::RenderWindow& window);
    

};

#endif