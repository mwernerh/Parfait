#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Player.h"
class EnemyManager{
    public:
        // Constructor
        EnemyManager();

        void update(float dt, Player& player);
        void draw(sf::RenderWindow& window);
        void configure(const std::string& texturePath,int maxEnemies, int health, float cooldown, float speed);

    private:
        float spawnTimer = 0.f;
        float spawnCooldown = 2.f;
        std::size_t maxEnemies = 5;
        float maxDistance = 1000.f;

        void spawnEnemy(const sf::Vector2f& playerPos);

        // add handleSpellCollisions and handlePlayerCollisions
        // Member Variables
        int baseHealth; // Base health of each enemy
        float enemySpeed; // Speed of the enemies
        std::vector<Enemy> enemies; // List of enemies
        std::shared_ptr<sf::Texture> enemyTexture; // Texture for the enemies
        

};

#endif
