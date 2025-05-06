#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include "HitboxManager.h"
#include "ScoreManager.h"
#include "AudioManager.h"
#include "Player.h"

/**
 * @author Isveydi Moreno
 * 
 */

constexpr unsigned int NUM_ANIM_FRAMES = 6;


// ENEMY is a class that represents an enemy in the game.

// Enemy constructor
Enemy::Enemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, [[maybe_unused]] int maxHealth, int scoreValue, float scaleX, float scaleY): attackHitbox(), colliderHitbox(this, std::bit_cast<ColliderHitbox::HitFuncGeneric>(&Enemy::takeDamage)) {
    this->texture = texture;
    sprite.setTexture(*texture); // dereference shared_ptr

    // Set the texture rect to the single base sprite.
    sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / NUM_ANIM_FRAMES, sprite.getTextureRect().height));
    sprite.setPosition(x, y);

    // Set the origin of the sprite to the center of the sprite (its 64x64)
    sprite.setOrigin(0, 0); 
    this->speed = speed;
    this->health = health;
    this->maxHealth = health;
    this->scoreValue = scoreValue;
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    sprite.setScale(scaleX, scaleY); // NOTE: this doesnt let the sprites show up properly in the game

    colliderHitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y); // Set the position of the collider hitbox
    colliderHitbox.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height)); // Set the size of the collider hitbox

    colliderHitbox.setFillColor(sf::Color(0, 255, 0, 127)); // Set the color of the collider hitbox to green with 50% opacity

    attackHitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y); // Set the position of the attack hitbox
    attackHitbox.setSize(sf::Vector2f(10, 10));
    attackHitbox.setOrigin(- sprite.getGlobalBounds().width, - sprite.getGlobalBounds().height /2);
    attackHitbox.setFillColor(sf::Color(255, 0, 0, 127));


}

// UPDATE THE ENEMY
//updates enemy movement and animation
void Enemy::update(float dt, sf::Vector2f playerPos){
    float direction = playerPos.x - sprite.getPosition().x; // Get the direction the enemy should move in

     // Calculate the length of the direction vector
    sprite.move({direction * speed * dt, 0}); // Move the enemy in the direction of the player
    colliderHitbox.move({direction * speed * dt, 0}); // Move the collider hitbox in the same direction
    attackHitbox.move({direction * speed * dt, 0}); // Move the attack hitbox in the same direction
    HitboxManager::RegisterColliderHitbox(&colliderHitbox);

    validDistance(playerPos); // Check if the enemy is too far away from the player
    //TODO: CHANGE SCALE FOR EACH ENEMY
    if (sprite.getPosition().x < playerPos.x){ //flip the sprite
        sprite.setScale(scaleX, scaleY); // Set the scale of the sprite to 6x
        colliderHitbox.setScale(1.f, 1.f); // Set the scale of the collider hitbox to 1x
        attackHitbox.setScale(1.f, 1.f); // Set the scale of the attack hitbox to 1x
    } else {
        sprite.setScale(-scaleX, scaleY); // Set the scale of the sprite to -6x
        colliderHitbox.setScale(-1.f, 1.f); // Set the scale of the collider hitbox to -1x
        attackHitbox.setScale(-1.f, 1.f); // Set the scale of the attack hitbox to -1x
    }

    handleAnimation(direction, dt);
}

//draw
//updates graphics and animation of the enemy in the game
void Enemy::draw(sf::RenderWindow& window){ //renders enemy to the window
    window.draw(sprite);
    window.draw(colliderHitbox); // Draw the collider hitbox
    window.draw(attackHitbox); // Draw the attack hitbox
}

//valid distance
// checks how far nemy is from the player
void Enemy::validDistance(sf::Vector2f playerPos){
    float distance = std:: abs(playerPos.x - sprite.getPosition().x); // Get distance from player

    if (distance > 1000.f) { // If the distance is greater than 1000 // MAYBE CHANGE DISTANCE FOR RAT
        killEnemy(); // Kill the enemy
    }
}   

//GET GLOBAL BOUNDS
sf::FloatRect Enemy::getGlobalBounds(){
    return sprite.getGlobalBounds();
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
        AudioManager::StartCameraSound("./assets/aud/e_dg_atk.wav"); //MAYBE CHANGE SOUND FOR EACH ENEMY TYPE
    }
    return instance -> getHealth(); // Return remaining health
}


//HANDLE ANIMATION
//handles the animation of the enemy in each frame
void Enemy::handleAnimation([[maybe_unused]] int direction, float dt){
    timeSinceLastFrame += dt; // Increment the elapsed time

    if (timeSinceLastFrame > timePerFrame) // If the elapsed time is greater than the time per frame
    {
        if (currentFrame == NUM_ANIM_FRAMES) // If the current frame is the last frame of the animation
        {
            currentFrame = 0; // Reset the current frame to the first frame
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * sprite.getTextureRect().width, 0, sprite.getTextureRect().width, sprite.getTextureRect().height)); // Set the texture rect to the current frame
        currentFrame++; // Increment the current frame
        timeSinceLastFrame = 0.f; // Reset the elapsed time
    }
    
}

int Enemy::getScoreValue() const {
    return scoreValue;
}

//kill enemy
void Enemy::killEnemy(){
    if (getHealth()<= 0){
        ScoreManager::AddScore(this-> getScoreValue()); // Add score to player //REMOVE & ADD TO EACH ENEMY
        std::cout<<"Score Value added"<< scoreValue <<std::endl;
    }
    
    alive = false; // Sets alive to false
}


//IS ALIVE
bool Enemy::isAlive() const{ 
    return alive;
}
/*
void Enemy::attackPlayer(Player& player){
 //will call attckHitbox fucntion
} */