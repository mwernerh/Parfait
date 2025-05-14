#include <SFML/Graphics.hpp>
#include "BirdEnemy.h"
#include <memory>
#include <cmath>
#include "Gamestate_Operators/Static/PlayerManager.h"

/**
 * @author Isveydi Moreno
 * 
 */

 BirdEnemy::BirdEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth)
 : Enemy(x, y, texture, speed, health, maxHealth, 50, 6.f, 6.f),
    originalPosition (x, y - 500), currentState(State::Idle), moveSpeed(speed), attackDuration(1.0f) {
        timer.restart();
        sprite.move(0.f, -500.f);
        attackHitbox.move(0.f, -500.f);
        colliderHitbox.move(0.f, -500.f);
        
    }


// Update the enemy
void BirdEnemy::update() {
    Enemy::update();
    sf::Vector2f playerPos = PlayerManager::GetPlayer().getPosition();

    //Check if the player is within a certain distance
    if (!validDistance(playerPos)) {
        currentState = State::Idle; // Set to idle state if player is too far
        return; // do not update if player is too far
    }
    
    float dt = timer.getElapsedTime().asSeconds();
    std::cout<< "BirdEnemy position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;


    switch (currentState) {
        case State::Idle:
            if (dt > 1.f) {
                currentState = State::MovingDown; // Change to moving down state
                timer.restart();
            }
            break;

        case State::MovingDown:
            if (sprite.getPosition().y < playerPos.y) {
                sprite.move(0.f, moveSpeed * dt); // Move down
            } else {
                currentState = State::Attacking; // Change to attacking state
                timer.restart();
            }
            break;

        case State::Attacking:
            if (dt > attackDuration) {
                currentState = State::MovingUp; // Change to moving up state
                attackTimer = attackDuration;
                timer.restart();
            }
            break;

        case State::MovingUp:
            if (sprite.getPosition().y > originalPosition.y) {
                sprite.move(0.f, -moveSpeed * dt); // Move up
            } else {
                currentState = State::Idle; // Change to idle state
                timer.restart();
            }
            break;
    }
 
}

void BirdEnemy::handleAnimation(int direction, float dt) {
    // Custom animation logic for BirdEnemy
    Enemy::handleAnimation(direction, dt);
    // use a timer to animate the sprite to move down (timer will be higher than 0) and have its vertical position change to the cat's position, have it stay there for a few seconds, then move back up. 
}

// Attack logic
void BirdEnemy::BirdAttack() {
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
