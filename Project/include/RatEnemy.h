#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "AnimatedSprite.h"
#include "Enemy.h"

/**
 * @author Isveydi Moreno
 * 
 */

 class RatEnemy : public Enemy {
    public:
        // Constructor
        RatEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth);

        // Update the enemy
        void update() override;

        void draw(sf::RenderWindow& window) override;

        void RatAttack();

    protected:
        float attackTimer = 1.5f;
        bool hasAttacked = false;
        AnimatedRatSprite1 sprite;
 };