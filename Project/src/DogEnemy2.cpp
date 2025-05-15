#include <SFML/Graphics.hpp>
#include "DogEnemy2.h"
#include <memory>
#include <cmath>
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/InputManager.h"

DogEnemy2::DogEnemy2(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth)
    : Enemy(x, y, texture, speed, health, maxHealth, 100, 6.f, 6.f) {
        attackHitbox.SetAttackerType(AttackHitbox::AttackerType::DOG_BLACK);

        sprite.setPosition(x, y);
        sprite.setScale(scaleX, scaleY); // NOTE: this doesnt let the sprites show up properly in the game
        sprite.SetAnimation<"Walk">();

        colliderHitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y); // Set the position of the collider hitbox
        colliderHitbox.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height)); // Set the size of the collider hitbox

        colliderHitbox.setFillColor(sf::Color(0, 255, 0, 127)); // Set the color of the collider hitbox to green with 50% opacity

        attackHitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y - (sprite.getGlobalBounds().height / 2.0f)); // Set the position of the attack hitbox
        attackHitbox.setSize(sf::Vector2f(50.0f, 50.0f));
        attackHitbox.setOrigin(- sprite.getGlobalBounds().width, - sprite.getGlobalBounds().height /2);
        attackHitbox.setFillColor(sf::Color(255, 0, 0, 127));
}

// Update the enemy
void DogEnemy2::update() {
    Enemy::update();
    sprite.Update();
    UpdateSprite(&sprite);
    validDistance(PlayerManager::GetPlayer().getPosition()); // Check if the enemy is too far away from the player

    if(std::abs(PlayerManager::GetPlayer().getPosition().x - sprite.getPosition().x) <= 500.0f) {
        DogAttack2();
     } 
}

void DogEnemy2::DogAttack2() {
    attackHitbox.isActive = false;

    attackTimer -= InputManager::GetDeltaTime();

    if (attackTimer < 0.5f && !hasAttacked) {
        attackHitbox.isActive = true;
        hasAttacked = true;

        AudioManager::StartCameraSound("e_dg_atk", 0.5f);

    }
    if (attackTimer <= 0.0f) {
        attackTimer = 1.5f;
        hasAttacked = false;
    }
}

void DogEnemy2::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}