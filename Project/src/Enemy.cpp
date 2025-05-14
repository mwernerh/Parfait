#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <iostream>
#include <memory>
#include <cmath>
#include "Framework_Managers/InputManager.h"
#include "Gamestate_Operators/Static/HitboxManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Gamestate_Operators/Static/ScoreManager.h"
#include "Framework_Managers/AudioManager.h"

/**
 * @author Isveydi Moreno
 * 
 */


// ENEMY is a class that represents an enemy in the game.

// Enemy constructor
Enemy::Enemy([[maybe_unused]] float x, [[maybe_unused]] float y, [[maybe_unused]] std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth, int scoreValue, float scaleX, float scaleY): attackHitbox(AttackHitbox::AttackerType::UNKNOWN), colliderHitbox(this, std::bit_cast<ColliderHitbox::HitFuncGeneric>(&Enemy::takeDamage)) {
    // Set the origin of the sprite to the center of the sprite (its 64x64)
    this->speed = speed;
    this->health = health;
    this->maxHealth = maxHealth;
    this->scoreValue = scoreValue;
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}

// UPDATE THE ENEMY
//updates enemy movement and animation
void Enemy::update(){
    colliderHitbox.SetParent(this);
    HitboxManager::RegisterColliderHitbox(&colliderHitbox);
    HitboxManager::RegisterAttackHitbox(&attackHitbox);
}

void Enemy::UpdateSprite(sf::Sprite* sprite) {
    const sf::Vector2f& playerPos = PlayerManager::GetPlayer().getPosition();
    const f32 dt = InputManager::GetDeltaTime();

    float direction = playerPos.x - sprite->getPosition().x; // Get the direction the enemy should move in

     // Calculate the length of the direction vector
    sprite->move({direction * speed * dt, 0}); // Move the enemy in the direction of the player
    colliderHitbox.move({direction * speed * dt, 0}); // Move the collider hitbox in the same direction
    attackHitbox.move({direction * speed * dt, 0}); // Move the attack hitbox in the same direction
    

    validDistance(playerPos); // Check if the enemy is too far away from the player
    //TODO: CHANGE SCALE FOR EACH ENEMY
    if (sprite->getPosition().x < playerPos.x){ //flip the sprite
        sprite->setScale(scaleX, scaleY); // Set the scale of the sprite to 6x
        colliderHitbox.setScale(1.f, 1.f); // Set the scale of the collider hitbox to 1x
        attackHitbox.setScale(1.f, 1.f); // Set the scale of the attack hitbox to 1x
    } else {
        sprite->setScale(-scaleX, scaleY); // Set the scale of the sprite to -6x
        colliderHitbox.setScale(-1.f, 1.f); // Set the scale of the collider hitbox to -1x
        attackHitbox.setScale(-1.f, 1.f); // Set the scale of the attack hitbox to -1x
    }
}

//draw
//updates graphics and animation of the enemy in the game
void Enemy::draw(sf::RenderWindow& window){ //renders enemy to the window
    window.draw(colliderHitbox); // Draw the collider hitbox
    window.draw(attackHitbox); // Draw the attack hitbox
}

//valid distance
// checks how far nemy is from the player
bool Enemy::validDistance(sf::Vector2f playerPos){
    float distance = std:: abs(playerPos.x - colliderHitbox.getPosition().x); // Get distance from player

    if (distance > 1000.f) { // If the distance is greater than 1000 // MAYBE CHANGE DISTANCE FOR RAT
        killEnemy(); // Kill the enemy
    }
    return true;
}

//GET GLOBAL BOUNDS
sf::FloatRect Enemy::getGlobalBounds(){
    return colliderHitbox.getGlobalBounds();
}

//GETTER AND SETTER FOR HEALTH
int Enemy::getHealth(){
    return health;
}

void Enemy::setHealth(int health){
    this->health = health;
}

//TAKE DAMAGE
int Enemy::takeDamage(Enemy* instance, [[maybe_unused]] const AttackHitbox* attacker){
    std::cout << "Enemy take damage function running: " << std::hex << instance << std::endl;
    instance-> setHealth(instance-> getHealth()- 1); // Decrease health by 1
    if (instance-> getHealth() <= 0){ // If health is less than or equal to 0
        instance-> killEnemy();
    }
    return instance -> getHealth(); // Return remaining health
}

int Enemy::getScoreValue() const {
    return scoreValue;
}

//kill enemy
void Enemy::killEnemy(){
    if (getHealth()<= 0){
        ScoreManager::AddScore(this-> getScoreValue()); // Add score to player //REMOVE & ADD TO EACH ENEMY
    }
    
    alive = false; // Sets alive to false
}


//IS ALIVE
bool Enemy::isAlive() const{ 
    return alive;
}
