#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

/** 
 *
 * @author Izzy Carlson
 *
 * Handles everything relating to the player i.e. set-up, attack, movement, etc.
 *
 **/

Player::Player(std::string texturePath, float speed) : at(), co(this, std::bit_cast<ColliderHitbox::HitFuncGeneric>(&Player::takeDamage))
{

	// load player texture from file
	if (!texture.loadFromFile(texturePath))
	{
		std::cerr << "Error loading texture!" << std::endl;
	}

	sprite.setTexture(texture); // set player texture to the sprite
	sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT)); // set texture rect based on player height and width
	sprite.setPosition(516, 550); // set player position on screen
	sprite.setOrigin(0,0); // set player origin to 0,0 for ease of flipping & flipping hitboxes
	sprite.setScale(PLAYER_SCALE, PLAYER_SCALE); // set player scale so its an appropriate size for the screen
	this->speed = speed; // set player speed
	
	// set up collider hitbox
	co.setSize({32.f * PLAYER_SCALE, 20.f * PLAYER_SCALE}); // set-up size same as the player
	co.setOrigin(0,0);
	co.setPosition(516, 550); 
	co.setFillColor(sf::Color(0, 255, 0, 127)); // GREEN WITH 50% OPACITY

	// set up attacker hitbox
	at.setSize ({(float)(OFFSET * PLAYER_SCALE), 20.f * PLAYER_SCALE}); // set about half the size as player, and put directly infront of the player
	at.setOrigin(0,0);
	at.setPosition(516 + ((PLAYER_WIDTH - OFFSET) * PLAYER_SCALE), 550);
	at.setFillColor(sf::Color(255, 0, 0, 127)); // RED WITH 50% OPACITY
	
	// set up text for displaying player health
	playerHealthText.setString("Health: ");
	playerHealthText.setCharacterSize(18); // smaller than score text
	playerHealthText.setFillColor(sf::Color::White);
	playerHealthText.setOutlineThickness(4.f);
	playerHealthText.setOutlineColor(sf::Color::Black);
	playerHealthText.setPosition(0,0);

}

sf::Vector2f Player::getPosition()
{
	return sprite.getPosition();
}

sf::FloatRect Player::getGlobalBounds()
{
	return sprite.getGlobalBounds();
}

AttackHitbox& Player::getAttackHitbox()
{
	return at;
}

ColliderHitbox& Player::getColliderHitbox()
{
	return co;
}

void Player::Attack(float dt)
{
	// TODO: Can be updated to add sound or change sprite when attacking
	// check attack key (K) is pressed
	if (InputManager::IsKeyPressed(sf::Keyboard::Scancode::K))
	{
			at.isActive = true; // set attack hitbox to active
			isAttacking = true;
	}

	if(attackTimer < 0.3f) {
		at.isActive = false;
	}

	// Only decrease attack time if player is trying to attack
	if(isAttacking)
		attackTimer -= dt; // decrease attack timer using delta time

	// once attack timer reaches zero, disable attack hitbox and reset attack timer
	if (attackTimer <= 0.0f)
	{
		isAttacking = false;
		attackTimer = 0.3f;
	}
}

void Player::draw(sf::RenderWindow& window)
{
	// draw colliding and attacking hitboxes
	window.draw(co);
	window.draw(at);

	// draw player
	window.draw(sprite);

	// draw player health text
	window.draw(playerHealthText);
}

// direction player is facing; used to flip sprite if needed
int direction = 1;

void Player::update(float dt)
{
	handleInput(dt);
	timeSinceLastHit += dt;

	// make sure collider hitbox stays on sprite
	if (direction == -1)
		co.setPosition({sprite.getPosition().x - (32 * PLAYER_SCALE), sprite.getPosition().y});
	else
		co.setPosition({sprite.getPosition().x, sprite.getPosition().y});
	
	// make sure attacker hitbox stays directly in front of sprite
	if (direction == -1)
		at.setPosition({sprite.getPosition().x - (PLAYER_WIDTH * PLAYER_SCALE), sprite.getPosition().y});
	else
		at.setPosition({sprite.getPosition().x + ((PLAYER_WIDTH - OFFSET) * PLAYER_SCALE), sprite.getPosition().y});
	
	Attack(dt);
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
	if (InputManager::IsKeyHeld(sf::Keyboard::Scancode::D))
	{
		direction = 1;
		movement.x += speed;
	}
	sprite.move(movement * speed * dt);
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

int Player::getHealth() const
{
	return health;
}

void Player::takeDamage(Player* const instance, const AttackHitbox* const attacker)
{
	// player currently invincible -- can attack but not take damage. will be implemented in the continuation of this project
 	(void)attacker;
	(void)instance;
}

bool Player::canTakeDamage() const
{
	return timeSinceLastHit >= damageCooldown;
}

bool Player::canAttack() const
{
	// TODO: true for now (player can always attack). can be updated to have a cooldown on player attacks
	return true;
}
