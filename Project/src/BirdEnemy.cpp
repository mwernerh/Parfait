#include <SFML/Graphics.hpp>
#include "BirdEnemy.h"
#include <memory>
#include <cmath>
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/InputManager.h"
#include "Framework_Managers/InputManager.h"

/**
 * @author Isveydi Moreno
 * 
 */

 BirdEnemy::BirdEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth)
 : Enemy(x, y, texture, speed, health, maxHealth, 50, 6.f, 6.f),
    originalPosition (x, y - 500), currentState(State::Idle), moveSpeed(speed), attackDuration(1.0f) {
    attackHitbox.SetAttackerType(AttackHitbox::AttackerType::BIRD);
    sprite.setPosition(x, y);
    sprite.setScale(scaleX, scaleY); // NOTE: this doesnt let the sprites show up properly in the game
    sprite.SetAnimation<"Walk">();

    colliderHitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y); // Set the position of the collider hitbox
    colliderHitbox.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height)); // Set the size of the collider hitbox

    colliderHitbox.setFillColor(sf::Color(0, 255, 0, 127)); // Set the color of the collider hitbox to green with 50% opacity

    attackHitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y - (sprite.getGlobalBounds().height / 2.0f)); // Set the position of the attack hitbox
    attackHitbox.setSize(sf::Vector2f(100.0f, 100.0f));
    attackHitbox.setOrigin(- sprite.getGlobalBounds().width, - sprite.getGlobalBounds().height /2);
    attackHitbox.setFillColor(sf::Color(255, 0, 0, 127));

    timer.restart();
    sprite.move(0.f, -500.f);
    attackHitbox.move(0.f, -500.f);
    colliderHitbox.move(0.f, -500.f);

    attackHitbox.SetAttackerType(AttackHitbox::AttackerType::BIRD);
        
    }


// Update the enemy
void BirdEnemy::update() {
    Enemy::update();

    sprite.Update();
    UpdateSprite(&sprite);

    sf::Vector2f playerPos = PlayerManager::GetPlayer().getPosition();

    //Check if the player is within a certain distance
    if (!validDistance(playerPos)) {
        currentState = State::Idle; // Set to idle state if player is too far
        return; // do not update if player is too far
    }
    
    float dt = timer.getElapsedTime().asSeconds();


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
                attackHitbox.move(0.f, moveSpeed * dt);
                colliderHitbox.move(0.f, moveSpeed * dt);
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
            if(std::abs(PlayerManager::GetPlayer().getPosition().x - sprite.getPosition().x) <= 500.0f) {
                BirdAttack();
             } 
            break;

        case State::MovingUp:
            if (sprite.getPosition().y > originalPosition.y) {
                sprite.move(0.f, -moveSpeed * dt); // Move up
                attackHitbox.move(0.f, -moveSpeed * dt);
                colliderHitbox.move(0.f, -moveSpeed * dt);
            } else {
                currentState = State::Idle; // Change to idle state
                timer.restart();
            }
            break;
    }
 
}

// Attack logic
void BirdEnemy::BirdAttack() {
    attackHitbox.isActive = false;

    attackTimer -= InputManager::GetDeltaTime();

    if (attackTimer < 0.5f && !hasAttacked) {
        attackHitbox.isActive = true;
        hasAttacked = true;

        AudioManager::StartCameraSound("e_bd_atk", 0.5f);

    }
    if (attackTimer <= 0.0f) {
        attackTimer = 1.5f;
        hasAttacked = false;
    }
}

void BirdEnemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
