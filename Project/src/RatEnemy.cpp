#include <SFML/Graphics.hpp>
#include "RatEnemy.h"
#include <memory>
#include <cmath>
#include "Gamestate_Operators/Static/PlayerManager.h"

/**
 * @author Isveydi Moreno
 * 
 */

 RatEnemy::RatEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth)
 : Enemy(x, y, texture, speed, health, maxHealth, 100, 6.f, 6.f) {
}

// Update the enemy
void RatEnemy::update() {
 Enemy::update();
 validDistance(PlayerManager::GetPlayer().getPosition()); // Check if the enemy is too far away from the player
}

void RatEnemy::handleAnimation(int direction, float dt) {
    // Custom animation logic for RatEnemy
    Enemy::handleAnimation(direction, dt);
}

// Attack function
void RatEnemy::RatAttack() {
    HitboxManager::RegisterAttackHitbox(&attackHitbox);
    attackHitbox.isActive = false;

    if (attackTimer < 0.5f && !hasAttacked) {
        attackHitbox.isActive = true;
        hasAttacked = true;

    }
    if (attackTimer <= 0.0f) {
        hasAttacked = false;
    }
}