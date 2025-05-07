#ifndef DOGENEMY_H
#define DOGENEMY_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Hitboxes.h"
#include "Enemy.h"

/**
 * @author Isveydi Moreno
 * 
 */

 class DogEnemy : public Enemy {
    public:
        // Constructor
        DogEnemy(float x, float y, std::shared_ptr<sf::Texture> texture, float speed, int health, int maxHealth);

        // Update the enemy
        void update(float dt, sf::Vector2f playerPos) override;

        // Handle animation
        void handleAnimation(int direction, float dt) override;

 };

#endif