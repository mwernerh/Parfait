#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

Player::Player(std::string texturePath, float speed) : at(), co(this, std::bit_cast<ColliderHitbox::HitFuncGeneric>(&Player::takeDamage))
{

	// Load texture from file
	if (!texture.loadFromFile(texturePath))
	{
		std::cerr << "Error loading texture!" << std::endl;
	}

	// set the texture to the sprite
	sprite.setTexture(texture);
	
	// set texture rect based on idle sprite
	sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));

	// set position of the player
	sprite.setPosition(516, 640);

	// set player center to center of sprite
	sprite.setOrigin(PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);

	sprite.setScale(PLAYER_SCALE, PLAYER_SCALE); 

	// set player speed
	this->speed = speed;
	
//	at.setColor();
//	at.setPosition();
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
	if (InputManager::IsKeyHeld(sf::Keyboard::Scancode::A))
	{
		direction = -1;
		movement.x -= speed;
	}
	if (InputManager::IsKeyHeld(sf::Keyboard::Scancode::D)) // moving right
	{
		direction = 1;
		movement.x += speed;
	}
	sprite.move(movement * speed * dt);
	// TODO: update hitboxes
	handleAnimation(direction, dt, NUM_FRAMES_WALK);
}

void Player::handleAnimation(int direction, float dt, int numFrames)
{
	timeSinceLastFrame += dt;

	if (timeSinceLastFrame > timePerFrame)
	{
		if (currentFrame == numFrames)
		{
			currentFrame = 0;
		}

		// update texture rect to show current frame
		sprite.setTextureRect(sf::IntRect(currentFrame * PLAYER_WIDTH, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
			
		// flip sprite
		sprite.setScale(PLAYER_SCALE * direction, PLAYER_SCALE); 

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
void Player::takeDamage(Player* const instance, const AttackHitbox* const attacker)
{
	// ignore these for now just so i stop getting a warning
 	(void)attacker;
	(void)instance;
}

// check if player can take damage
bool Player::canTakeDamage() const
{
	return timeSinceLastHit >= damageCooldown;
}
