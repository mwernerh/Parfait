#include <iostream>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <string>
#include <memory>

Enemy::Enemy(float x, float y, std::shared_ptr<sf::Texture> texture) {
    this-> texture = texture;
    sprite.setTexture(*texture);

    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setPosition(x, y);

    sprite.setOrigin(32,32);
    //Add speed, health, and max health 
    sprite.setScale(2.f, 2.f);
}