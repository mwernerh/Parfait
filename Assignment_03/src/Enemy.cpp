#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>


// ENEMY is a class that represents an enemy in the game.

// Enemy constructor
Enemy::Enemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth){
    this->texture = texture;
    sprite.setTexture(*texture); // dereference shared_ptr

    // Set the texture rect to the single base sprite.
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setPosition(x, y);

    // Set the origin of the sprite to the center of the sprite (its 64x64)
    sprite.setOrigin(32, 32);
    this->speed = speed;
    this->health = health;
    this->maxHealth = health;
    sprite.setScale(2.f, 2.f);

}

// UPDATE THE ENEMY
//updates enemy movement and animation
void Enemy::update(float dt, sf::Vector2f playerPos){
    sf::Vector2f direction = playerPos - sprite.getPosition(); // Get the direction the enemy should move in

    float temp = sqrt(direction.x * direction.x + direction.y * direction.y); // Calculate the length of the direction vector
    if (temp > 0) // If the length is greater than 0
    {
        direction /= temp; // Normalize the direction
        sprite.move(direction * speed * dt); // Move the enemy in the direction of the player
    }
    validDistance(playerPos); // Check if the enemy is too far away from the player

    int enemyDirection = 1; // Figure out if we are moving left or right, 1 right, -1 left
    if (sprite.getPosition().x < playerPos.x) // If the enemy is to the left of the player
    {
        enemyDirection = 1; // Move right
    }
    else
    {
        enemyDirection = -1; // Move left
    }

}

//draw
//updates graphics and animation of the enemy in the game
void Enemy::draw(sf::RenderWindow& window){ //renders enemy to the window
    window.draw(sprite);
}

//valid distance
// checks how far nemy is from the player
void Enemy::validDistance(sf::Vector2f playerPos){

    float distance = playerPos.x - sprite.getPosition().x; //get distance from player
    float length = sqrt(distance.x * distance.x + distance.y * distance.y); //calculate length of the distance vector
    
    if (length > 1000.f){ //if the distance is greater than 1000
        killEnemy(); //kill the enemy
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

int Enemy::takeDamage(){
    health -=1; // Decrease health by 1
    if (health <= 0){ // If health is less than or equal to 0
        killEnemy();
    }
    return health; // Return remaining health
}


//HANDLE ANIMATION
//handles the animation of the enemy in each frame
void Enemy::handleAnimation(int direction, float dt){
    timeSinceLastFrame += dt; // Increment the elapsed time

    if (timeSinceLastFrame > timePerFrame) // If the elapsed time is greater than the time per frame
    {
        if (currentFrame == 6) // If the current frame is the last frame of the animation
        {
            currentFrame = 0; // Reset the current frame to the first frame
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * 64, 0, 64, 64)); // Set the texture rect to the current frame
        currentFrame++; // Increment the current frame
        timeSinceLastFrame = 0.f; // Reset the elapsed time
    }
}
//kill enemy
void Enemy::killEnemy(){
    alive = false; // Sets alive to false
}


//IS ALIVE
bool Enemy::isAlive() const{ 
    return alive;
}