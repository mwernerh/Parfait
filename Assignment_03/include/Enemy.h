#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Hitboxes.h"

class Enemy{
    public:
    // Constructor
        Enemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth);

        //UPDATING
        //update the enemy
        void update(float dt, sf::Vector2f playerPos);

        //updates enemy movement and animation
        void draw(sf::RenderWindow& window);

        //HITBOX
        //get the hitbox of the enemy
        


        //CHECKING DISTANCE
        //check if the enemy is too far away from the player
        void validDistance(sf::Vector2f playerPos);

        //get the global bounds of the enemies sprite
        sf::FloatRect getGlobalBounds();

        //HEALTH AND DAMAGE
        //get the health of the enemy
        int getHealth();
        //set the health of the enemy
        void setHealth(int health);
        //take damage (returns remaining health)
        static int takeDamage(Enemy* instance, const AttackHitbox* attacker);

        //ANIMATION
        //handle animation
        void handleAnimation(int direction, float dt);

        //ALIVE AND KILL
        //kill the enemy
        void killEnemy();
        //check if the enemy is still alive
        bool isAlive() const;

        //get the enemies max health
        int getMaxHealth() const {
            return maxHealth; 
        }
        //x and y are the starting position



    private:
        std::shared_ptr<sf::Texture> texture; //Texture var for the enemy
        sf::Sprite sprite; // Sprite var for the enemy
        //Enemy attributes
        float speed;
        int health;
        int maxHealth;


        //Enemy Animation attributes
        int currentFrame = 0; // The current frame of the animation
        float timePerFrame = 0.2f; // Time per frame
        float timeSinceLastFrame = 0.0f; // Time since the last frame of the animation
        bool alive = true; // Is the enemy alive? 
        AttackHitbox attackHitbox;
        ColliderHitbox colliderHitbox;

};

#endif
