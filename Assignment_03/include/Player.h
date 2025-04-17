#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{

	public:
		// constructor prototype to initialize player
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
		void takeDamage(int amount);
		bool canTakeDamage() const;

		//const Hitbox& hitbox const;

	private:
		// player texture and sprite
		sf::Texture texture;
		sf::Sprite sprite;

		int currentFrame = 0; // current frame of the animation
		float timePerFrame = 0.2f; // 1/5 of a second per frame
		float timeSinceLastFrame = 0.0f; // time since last frame of the animation

		float speed = 100.f; // speed of the player

		int health = 10; // player health 

		// damage management
		float damageCooldown = 0.5f; // seconds
		float timeSinceLastHit = 0.0f;

		void handleAnimation(int direction, float dt);

		//Hitbox hitbox;
};

#endif
