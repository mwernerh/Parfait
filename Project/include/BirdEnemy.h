#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "AnimatedSprite.h"
#include "Enemy.h"
#include "SFML/Graphics/RenderWindow.hpp"

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

        void draw(sf::RenderWindow& window) override;

        void BirdAttack();

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
        //AttackHitbox attackHitbox;
        float attackDuration;
        float attackTimer = 0.0f;
        bool hasAttacked = false;
        AnimatedBirdSprite1 sprite;

 };
