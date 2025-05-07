#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Hitboxes.h"
/**
 * @author Isveydi Moreno
 * 
 */

class Enemy{
    public:
    // Constructor
        Enemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth, int scoreValue, float scaleX, float scaleY);
        virtual ~Enemy() = default; // Destructor
        //UPDATING
        //update the enemy
        virtual void update();

        //updates enemy movement and animation
        virtual void draw(sf::RenderWindow& window);

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
        //Get Score Value of the enemy
        int getScoreValue() const;


        //take damage (returns remaining health)
        static int takeDamage(Enemy* instance, const AttackHitbox* attacker);

        //ANIMATION
        //handle animation
        virtual void handleAnimation(int direction, float dt);

        //ALIVE AND KILL
        //kill the enemy
        void killEnemy();
        //check if the enemy is still alive
        bool isAlive() const;

        //void attackPlayer(Player& player);

        //get the enemies max health
        int getMaxHealth() const {
            return maxHealth; 
        }



        //set parent of the collider hitbox
        void setColliderParent(void* parent) { colliderHitbox.SetParent(parent); }



    private:
        std::shared_ptr<sf::Texture> texture; //Texture var for the enemy
        sf::Sprite sprite; // Sprite var for the enemy

        //Enemy attributes
        float speed;
        int health;
        int maxHealth;
        int scoreValue; // Score default value for the enemy
        float scaleX; // default scale 
        float scaleY; // default scale


        //Enemy Animation attributes
        int currentFrame = 0; // The current frame of the animation
        float timePerFrame = 0.2f; // Time per frame
        float timeSinceLastFrame = 0.0f; // Time since the last frame of the animation
        bool alive = true; // checks if enemy is alive
        AttackHitbox attackHitbox;
        ColliderHitbox colliderHitbox;

};
