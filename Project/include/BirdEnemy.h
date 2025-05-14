#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Enemy.h"

/**
 * @author Isveydi Moreno
 * 
 */

 class BirdEnemy : public Enemy {
    public:
        // Constructor
        BirdEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth);

        // Update the enemy
        void update() override;

        // Handle animation
        void handleAnimation(int direction, float dt) override;

    private:
        enum class State{
            Idle,
            MovingDown,
            Attacking,
            MovingUp
        };
        sf::Vector2f originalPosition;
        State currentState;
        sf::Clock timer; // Timer for state transitions
        float moveSpeed;
        float attackDuration;
        


 };
