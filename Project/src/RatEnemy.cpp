#include <SFML/Graphics.hpp>
#include "RatEnemy.h"
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

 RatEnemy::RatEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth)
 : Enemy(x, y, texture, speed, health, maxHealth, 100, 6.f, 6.f) {
}

// Update the enemy
void RatEnemy::update(float dt, sf::Vector2f playerPos) {
 Enemy::update(dt, playerPos);
 validDistance(playerPos); // Check if the enemy is too far away from the player
}

void RatEnemy::handleAnimation(int direction, float dt) {
    // Custom animation logic for RatEnemy
    Enemy::handleAnimation(direction, dt);
}