#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "AnimatedSprite.h"
#include "Enemy.h"

/**
 * @author Isveydi Moreno
 * 
 */

 class DogEnemy2 : public Enemy {
    public:
        // Constructor
        DogEnemy2(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth);

        // Update the enemy
        void update() override;
        
        void DogAttack2();

        void draw(sf::RenderWindow& window) override;

    protected:
        float attackTimer = 0.0f;
        bool hasAttacked = false;
        AnimatedDogSprite2 sprite;
 };