#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{

	public:
		// constructor prototype to initialize player
		Player(std::string texturePath);
		void draw(sf::RenderWindow& window);

	private:
		sf::Texture texture;
		sf::Sprite sprite;

		int currentFrame = 0; // current frame of the animation
		float timePerFrame = 0.2f; // 1/5 of a second per frame
		float timeSinceLastFrame = 0.0f;

		void handleAnimation(int direction, float dt);
};

#endif
