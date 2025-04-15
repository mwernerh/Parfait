#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

Player::Player(std::string texturePath)
{

	// Load texture from file
	if (!texture.loadFromFile(texturePath))
	{
		std::cerr << "Error loading texture!" << std::endl;
	}

	// set the texture to the sprite
	sprite.setTexture(texture);
	
	// set texture rect based on walking sprite (Cat 1) (sprite sheet for walking animation is 6 frames, 38 x 25)
	sprite.setTextureRect(sf::IntRect(0, 0, 38, 25));

	// set position of the player
	sprite.setPosition(400,300);

	// set player center to center of sprite
	sprite.setOrigin(19, 12);

	sprite.setScale(2,2); 
}

void Player::draw(sf::RenderWindow& window)
{
	// draw the player
	window.draw(sprite);
}

int direction = 1;
// no movement yet so direction stagnant

void Player::handleAnimation(int direction, float dt)
{
	timeSinceLastFrame += dt;

	if (timeSinceLastFrame > timePerFrame)
	{
		// 6 frames for cat 1 walk animation
		if (currentFrame == 6)
		{
			currentFrame = 0;
		}

		// update texture rect to show current frame
		sprite.setTectureRect(sf::IntRect(currentFrame * 38, 0, 38, 25));
		
		// then, update current frame
		currentFrame++; 
		
		// reset elapsed time
		timeSinceLastFrame = 0;

	}	
}
