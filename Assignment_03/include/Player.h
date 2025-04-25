#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Hitboxes.h"
#include "HitboxManager.h"
#include "InputManager.h"
#include "Enemy.h"

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
		
		// method to draw player to the screen
		void draw(sf::RenderWindow& window);

		void update(float dt);

		// handle player input
		void handleInput(float dt);
		
		// getters to get player position and global bounds
		sf::Vector2f getPosition();
		sf::FloatRect getGlobalBounds();

		// health management
		int getHealth() const;

		// damaging and attacking checks
		bool canTakeDamage() const;
		bool canAttack() const;

		// retrieve hitboxes
		AttackHitbox& getAttackHitbox();
		ColliderHitbox& getColliderHitbox();

		// function to player to attack
		void Attack(float dt);

	private:
		// player texture and sprite
		sf::Texture texture;
		sf::Sprite sprite;

		// set up hitboxes
		AttackHitbox at;
		ColliderHitbox co;
		
		// variables to handle animation
		int currentFrame = 0; // current frame of the animation
		float timePerFrame = 0.1f; // 1/10 of a second per frame
		float timeSinceLastFrame = 0.0f; // time since last frame of the animation
	
		float speed = 100.f; // speed of the player

		int health = 10; // player health 

		// damage management
		float damageCooldown = 0.5f; // seconds
		float timeSinceLastHit = 0.0f;

		float attackTimer = 0.3f; 

		void handleAnimation(int direction, float dt, int numFrames);

		static void takeDamage(Player* const instance, const AttackHitbox* const attacker);
};

#endif
