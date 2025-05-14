#include "EnemyManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/FontManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include <iostream>
#include <cmath>
#include <cstdlib>  // used for rand() spawn logic

/**
 * @author Isveydi Moreno
 * 
 */

//ENEMYMANAGER is a class that manages the enemies in the game.

//ENEMYMANAGER CONSTRUCTOR
EnemyManager::EnemyManager():spawnTimer(0.f), spawnCooldown(2.f), maxEnemies(2), baseHealth(1), enemySpeed(.1f){
}

//ENEMY MANAGER UPDATE
//Updates enemy based on the player position
void EnemyManager::update(){
    const sf::Vector2f& playerPos = PlayerManager::GetPlayer().getPosition(); // Get the player's position
    const f32 dt = InputManager::GetDeltaTime();

    spawnTimer += dt; // Increment the spawn timer

    //Spawn enemies if the spawn timer has reached the cooldown and the number of enemies is less than the maximum
    if (spawnTimer >= spawnCooldown && enemies.size() < maxEnemies){
        spawnEnemy(playerPos); // Spawn a new enemy
        spawnTimer = 0.f; // Reset the spawn timer

    }

    // Update each enemy
    for (auto& enemy : enemies){
        enemy.update(); //update the enemy's position
    }

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
        if(!enemy.isAlive())
            std::cout << "Erasing enemy: " << std::hex << &enemy << std::endl;
	return !enemy.isAlive(); // Remove enemies that are not alive
    }), enemies.end());
}


//ENEMY MANAGER SPAWN ENEMIES
// This function spawns enemies at a random side based on the player's position
void EnemyManager::spawnEnemy(const sf::Vector2f& playerPos){
    float buffer = 200.f;
    sf::Vector2f spawnPos;
    
    int side = rand() % 2; // Randomly choose a side to spawn the enemy
    switch(side){
        case 0: // Spawn on the left side
            spawnPos = playerPos + sf::Vector2f(-800 - buffer, 0);
            break;
        case 1: // Spawn on the right side
            spawnPos = playerPos + sf::Vector2f(800 + buffer, 0);
            break;
    }

    enemies.emplace_back(spawnPos.x, spawnPos.y - 16 * 6, enemyTexture, enemySpeed, baseHealth, baseHealth, 100, 6.f, 6.f); // Create a new enemy at the spawn position
}

//ENEMY MANAGER DRAW
//updates graphics and animation of the enemy in the game
void EnemyManager::draw(sf::RenderWindow& window){
    try{
        for (auto& enemy : enemies){ // Iterate through the list of enemies
            enemy.draw(window); // Draw each enemy
        }
    } catch (const std::exception& e){
        std::cerr << "Error drawing enemy: " << e.what() << std::endl;
    }
}

void EnemyManager::drawHealthBar(sf::RenderWindow& window){
    for (auto& enemy : enemies){ // Iterate through the list of enemies
        enemy.draw(window); // Draw each enemy
        sf::Text healthText;
        healthText.setFont(FontManager::GetFont("Akira Expanded Demo")); // Set the font for the text
        healthText.setCharacterSize(24); // Set the character size
        healthText.setFillColor(sf::Color::White); // Set the text color
        healthText.setString(std::to_string(enemy.getHealth())); // Set the text to the enemy's health
        std::to_string(enemy.getMaxHealth());

        sf::FloatRect bounds = enemy.getGlobalBounds(); // Get the bounds of the enemy
        healthText.setPosition(bounds.left + bounds.width / 2 - healthText.getGlobalBounds().width / 2, bounds.top - 20); // Set the position of the text above the enemy
        window.draw(healthText); // Draw the health text
    }
}

//ENEMY MANAGER CONFIGURE
// This function configures the enemy manager with the specified parameters
void EnemyManager::configure(const std::string& texturePath, int maxEnemies, int health, float cooldown, float speed) { 
    this->maxEnemies = maxEnemies; // Maximum number of enemies
    this->spawnCooldown = cooldown; // Time between enemy spawns
    this->baseHealth = health; // Base health of each enemy
    this->enemySpeed = speed; // Speed of each enemy
    this->enemies.reserve(maxEnemies);
    this->enemies.clear();

    enemyTexture = std::make_shared<sf::Texture>(); // Create a shared pointer to the texture
    if (!enemyTexture->loadFromFile(texturePath)){ // Load the texture from the specified file
        std::cerr << "Failed to load enemy texture: " << texturePath << std::endl;
    }
    else{
        std::cout << "Enemy texture loaded successfully: " << texturePath << std::endl;
    }
}
