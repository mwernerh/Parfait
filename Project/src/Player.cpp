#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Framework_Managers/InputManager.h"
#include "SFML/System/Vector2.hpp"
#include "Framework_Managers/FontManager.h"
#include "Framework_Managers/AudioManager.h"

/** 
 *
 * @author Izzy Carlson
 *
 * Handles everything relating to the player i.e. set-up, attack, movement, etc.
 *
 **/

Player::Player() : at(AttackHitbox::AttackerType::PLAYER), co(this, std::bit_cast<ColliderHitbox::HitFuncGeneric>(&Player::takeDamage))
{
	sprite.SetAnimation<"Idle">();
	sprite.setPosition(516, 550); // set player position on screen
	sprite.setOrigin(0,0); // set player origin to 0,0 for ease of flipping & flipping hitboxes
	sprite.setScale(PLAYER_SCALE, PLAYER_SCALE); // set player scale so its an appropriate size for the screen
	this->speed = 20; // set player speed

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
	playerHealthText.setFont(FontManager::GetFont("Akira Expanded Demo"));
	playerHealthText.setString("Health: " + std::to_string(health));
	playerHealthText.setCharacterSize(18); // smaller than score text
	playerHealthText.setFillColor(sf::Color::White);
	playerHealthText.setOutlineThickness(4.f);
	playerHealthText.setOutlineColor(sf::Color::Black);
	playerHealthText.setPosition(10, 30); // set to top left corner, slightly off center so it wont be right in the corner
//	showText = false; // initialize show text as false since we dont want text on title screen
}

sf::Vector2f Player::getPosition() const
{
	return sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f& pos)
{
	sprite.setPosition(pos);
	at.setPosition(pos);
	co.setPosition(pos);
}

sf::FloatRect Player::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

const AttackHitbox& Player::getAttackHitbox() const
{
	return at;
}

const ColliderHitbox& Player::getColliderHitbox() const
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
			sprite.SetAnimation<"Attack">();
			// play a swiping sound every hit and an angry cat sound every 4th hit
			numHits++;
			AudioManager::StartCameraSound("p_atk1");
			if (numHits == 4)
			{
				AudioManager::StartCameraSound("p_ct_atk");
				numHits = 0;
			}
	}

	if(attackTimer < 0.3f) {
		at.isActive = false;
	}

	// Only decrease attack time if player is trying to attack
	if(isAttacking)
	{
		attackTimer -= dt; // decrease attack timer using delta time
	}

	// once attack timer reaches zero, disable attack hitbox and reset attack timer
	if (attackTimer <= 0.0f)
	{
		isAttacking = false;
		attackTimer = 0.3f;
		sprite.SetAnimation<"Idle">();
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
	if (showText)
		window.draw(playerHealthText);
}

// direction player is facing; used to flip sprite if needed
int direction = 1;

void Player::update(float dt)
{
	handleInput(dt);
	timeSinceLastHit += dt;
	
	// if player is poisoned by rat, health needs to slowly decrement per frame until poison timer is zero
	if (poisonTimer > 0)
	{
		// decrement poison timer
		poisonTimer -= dt;

		// decrement health by 1
		health--;

		// increment counter
		counter++;
		
		// for cat being poisoned, cat hurt animation every 4 frames
		if (counter == 4)
		{
			// set sprite to hurt
			//sprite.SetAnimation<"Hurt">();
			//sprite.Update();

			// reset counter
			counter = 0;
		}
	}

	// update health printed on the screen
	playerHealthText.setString("Health: " + std::to_string(health));

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

	// movement F->left, J->right for accessibility notches
	if (InputManager::IsKeyHeld(sf::Keyboard::Scancode::F))
	{
		direction = -1;
		movement.x -= speed;
		sprite.SetAnimation<"Walk">();
	}
	if (InputManager::IsKeyHeld(sf::Keyboard::Scancode::J))
	{
		direction = 1;
		movement.x += speed;
		sprite.SetAnimation<"Walk">();
	}
	sprite.move(movement * speed * dt);
	playerHealthText.move(movement*speed*dt);

	if (!InputManager::IsKeyHeld(sf::Keyboard::Scancode::F) && !InputManager::IsKeyHeld(sf::Keyboard::Scancode::J) && !isAttacking)
	{
		sprite.SetAnimation<"Idle">();
	}

	sprite.setScale(PLAYER_SCALE * direction, PLAYER_SCALE);
	
	sprite.Update();
}

int Player::getHealth() const
{
	return health;
}

void Player::setHealth(int health)
{
	this->health = health;
}

void Player::takeDamage(Player* const instance, const AttackHitbox* const attacker)
{
	// ensure player can take damage first
	if (instance->canTakeDamage())
	{
		// handle changing sprite animations when player is hurt
		//sprite.SetAnimation<"Hurt">();
		//sprite.Update();
	
		// player health decreases based on enemy (attacker) type
		switch (attacker->GetAttackerType())
		{
			// slower dog does slightly more damage than faster dog
			case AttackHitbox::AttackerType::DOG_BLACK:
				instance->setHealth(instance->getHealth() - 10);
				break;
			// faster dog does slightly less damage than slow dog
			case AttackHitbox::AttackerType::DOG_YELLOW:
				instance->setHealth(instance->getHealth() - 8); 
				break;
			// bird does slightly less damage than the dogs
			case AttackHitbox::AttackerType::BIRD:
				instance->setHealth(instance->getHealth() - 5);
				break;
			// poison timer is set, player health affected by poison is updated in update function
			case AttackHitbox::AttackerType::RAT:
				instance->poisonTimer = 3.f;
				break;
			default:
				break;
		}
		instance->timeSinceLastHit = 0.f; // reset time since last hit
	}

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

void Player::setShowText(bool showText)
{
	this->showText = showText;
}
