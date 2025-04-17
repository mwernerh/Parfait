#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

Player::Player(std::string texturePath, float speed)
{

	// Load texture from file
	if (!texture.loadFromFile(texturePath))
	{
		std::cerr << "Error loading texture!" << std::endl;
	}

	// set the texture to the sprite
	sprite.setTexture(texture);
	
	// set texture rect based on walking sprite (Cat 1) (sprite sheet for walking animation is 6 frames, 38 x 25)
	sprite.setTextureRect(sf::IntRect(0, 0, 32, 20));

	// set position of the player
	sprite.setPosition(400,300);

	// set player center to center of sprite
	sprite.setOrigin(16, 10);

	sprite.setScale(2,2); 

	// set player speed
	this->speed = speed;
	
	// define a hitbox for the player
	// hitbox = Hitbox({22.f, 20.f}, sf::Color(0, 255, 0, 100)); // light green color for player
}

// get the player's potion
sf::Vector2f Player::getPosition()
{
	return sprite.getPosition();
}

// get player's global bounds
sf::FloatRect Player::getGlobalBounds()
{
	return sprite.getGlobalBounds();
}

void Player::draw(sf::RenderWindow& window)
{
	// draw the player & hitbox
	window.draw(sprite);
	// hitbox.draw(window);
}

// direction player is facing; used to flip sprite if needed
int direction = 1;

void Player::update(float dt)
{
	handleInput(dt);
	timeSinceLastHit += dt;

	// set player hitbox with an offet (NOT sure about this number, may need to be adjusted)
	//hitbox.setPosition({sprite.getPosition().x, sprite.getPosition().y + 10});
}

void Player::handleInput(float dt)
{
	sf::Vector2f movement(0,0);

	// movement A->left, D->right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) // moving left
	{
		direction = -1;
		movement.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) // moving right
	{
		direction = 1;
		movement.x += speed;
	}

	// actually move the player
	sprite.move(movement * speed * dt);

	// handle the animation
	handleAnimation(direction, dt);
}

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
		sprite.setTextureRect(sf::IntRect(currentFrame * 38, 0, 38, 25));
			
		// flip sprite
		sprite.setScale(2 * direction, 2); 

		// then, update current frame
		currentFrame++; 
		
		// reset elapsed time
		timeSinceLastFrame = 0;

	}	
}

// get player health
int Player::getHealth() const
{
	return health;
}

// make player take damage
void Player::takeDamage(int amount)
{
	if (canTakeDamage())
	{
		health -= amount;
		std::cout << "Player took damage!" << std::endl;
		timeSinceLastHit = 0.0f;
	}
}

// check if player can take damage
bool Player::canTakeDamage() const
{
	return timeSinceLastHit >= damageCooldown;
}
