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
	sprite.setPosition(516, 550);

	// set player center to 0,0 -- makes it easier for flipping sprite and hitboxes
	sprite.setOrigin(0,0);

	sprite.setScale(PLAYER_SCALE, PLAYER_SCALE); 

	// set player speed
	this->speed = speed;
	
	// set up collider hitbox
	co.setSize({32.f * PLAYER_SCALE, 20.f * PLAYER_SCALE});
	co.setOrigin(0,0);
	co.setPosition(516, 550); 
	co.setFillColor(sf::Color::Red);

	// set up attacker hitbox
	at.setSize ({(float)(OFFSET * PLAYER_SCALE), 20.f * PLAYER_SCALE});
	at.setOrigin(0,0);
	at.setPosition(516 + ((PLAYER_WIDTH - OFFSET) * PLAYER_SCALE), 550); 
	at.setFillColor(sf::Color::Green);
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
	if (InputManager::IsKeyHeld(sf::Keyboard::Scancode::D)) // moving right
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

bool Player::canAttack() const
{
	// TODO: true for now (player can always attack). can be updated to have a cooldown on player attacks
	return true;
}
