#include <SFML/Graphics.hpp>
#include "BirdEnemy.h"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include "HitboxManager.h"
#include "ScoreManager.h"
#include "AudioManager.h"

/**
 * @author Isveydi Moreno
 * 
 */

 BirdEnemy::BirdEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth)
 : Enemy(x, y, texture, speed, health, maxHealth, 50, 6.f, 6.f) {
}

// Update the enemy
void BirdEnemy::update(float dt, sf::Vector2f playerPos) {
 Enemy::update(dt, playerPos);
 validDistance(playerPos); // Check if the enemy is too far away from the player
}

void BirdEnemy::handleAnimation(int direction, float dt) {
    // Custom animation logic for BirdEnemy
    Enemy::handleAnimation(direction, dt);
    // use a timer to animate the sprite to move down (timer will be higher than 0) and have its vertical position change to the cat's position, have it stay there for a few seconds, then move back up. 
}