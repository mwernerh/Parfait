#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Hitboxes.h"
#include "Gamestate_Operators/Static/HitboxManager.h"

/**
 *  @author Izzy Carlson
 * 
 *  @brief Member variables and functions regarding the player
 *
 **/

class Player
{

	public:
		// CONSTANTS
		const int PLAYER_SCALE = 6;
		const int PLAYER_WIDTH = 48;
		const int PLAYER_HEIGHT = 20;
		const int NUM_FRAMES_IDLE = 4;
		const int NUM_FRAMES_WALK = 6;
		const int OFFSET = 16; // there seems to be a 16 pixel whitespace offset between frames, needed to correctly position hitbox

		// constructor to initialize player
		Player(std::string texturePath, float speed);
		
		/**
		 * @brief draws player and hitboxes in the window
		 * 
		 * @param window to be drawn to
		 *
		 **/
		void draw(sf::RenderWindow& window);
		
		/**
		 * @brief intended to be used to update player per frame, handles direction, etc.
		 * 
		 * @param delta time 
		 *
		 **/
		void update(float dt);
	
		/** 
		 * @brief handles player input for movement
		 *
		 * @param delta time
		 *
		 **/
		void handleInput(float dt);
		
		/**
		 * @brief gets player position on the screen
		 * 
		 * @return SFML length 2 vector of player's x and y coordinates on the screen
		 *
		 **/
		sf::Vector2f getPosition() const;
		sf::FloatRect getGlobalBounds() const;

		/**
		 * @brief gets player's health
		 *
		 * @return int value of player's current health
		 **/
		int getHealth() const;

		/**
		 * @brief set's players health
		 * 
		 * @param amount to set health to
		 * 
		 **/
		 void setHealth(int health);

		/**
 		 * @brief checks whether player can take damage
 		 * 
		 * @return true if time since last attack is a certain val, false if time since last attack is not certain val
		 *
		 **/
		bool canTakeDamage() const;

		/**
		 * @brief checks whether player can attack
		 * 
		 * @returns currently always returns true
		 *
		 **/
		bool canAttack() const;

		/**
		 * @brief gets player's attacker hitbox
		 * 
		 * @returns player's attacker hitbox type AttackHitbox
		 **/
		const AttackHitbox& getAttackHitbox() const;

		/**
		 * @brief gets player's colliding hitbox
		 *
		 * @returns player's colliding hitbox type ColliderHitbox
		 **/
		const ColliderHitbox& getColliderHitbox() const;

		/**
		 * @brief player's attacking function, player can attack when key is pressed, activates attack hitbox
		 * 
		 * @param delta time
		 **/
		void Attack(float dt);

	private:
		// player texture and sprite
		sf::Texture texture;
		AnimatedCatSprite2 sprite;

		// set up hitboxes
		AttackHitbox at;
		ColliderHitbox co;
		
		// animation management
		int currentFrame = 0; // current frame of the animation
		float timePerFrame = 0.1f; // 1/10 of a second per frame
		float timeSinceLastFrame = 0.0f; // time since last frame of the animation
	
		float speed = 100.f; // speed of the player
		
		// health management
		int health = 100;

		// damage management
		float damageCooldown = 0.5f; // seconds
		float timeSinceLastHit = 0.0f;
		float poisonTimer = 0.0f; // starts at 0

		// attack management
		float attackTimer = 0.3f; 
		bool isAttacking = false;

		/**
		 * @brief allows player to take damage (currently does nothing will be updated in future version of project)
		 * 
		 * @param player instance
		 * @param what's attacking the player
		 **/
		static void takeDamage(Player* const instance, const AttackHitbox* const attacker);
};
