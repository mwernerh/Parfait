#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Player.h"

/**
 * @author Isveydi Moreno
 * 
 */
class EnemyManager{
    public:
        // Constructor
        EnemyManager();

        //ENEMY MANAGER IN GAME FUNCTIONS
        void update(float dt, Player& player); //Update Enemies
        void draw(sf::RenderWindow& window); //Draw Enemies
        void configure(const std::string& texturePath,int maxEnemies, int health, float cooldown, float speed); //Configure Enemies
    private:
        // Enemy Spawn Logic
        float spawnTimer = 0.f;
        float spawnCooldown = 2.f;
        std::size_t maxEnemies = 5;
        float maxDistance = 1000.f;

        void spawnEnemy(const sf::Vector2f& playerPos); // Spawn enemies

        // Member Variables
        int baseHealth; // Base health of each enemy
        float enemySpeed; // Speed of the enemies
        std::vector<Enemy> enemies; // List of enemies
        std::shared_ptr<sf::Texture> enemyTexture; // Texture for the enemies
        

};

#endif
