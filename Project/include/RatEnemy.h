#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
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

        // Handle animation
        void handleAnimation(int direction, float dt) override;

        void RatAttack();

    protected:
        float attackTimer = 0.0f;
        bool hasAttacked = false;

 };