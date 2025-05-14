#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"

/*
 * @author Izzy Carlson
 * 
 * @brief Member variables and functions relating to the NPC. In this case, the NPC is on the title screen and will provide
 *  direction for controls to the user on the home page of the game.
 * 
 **/

class NPC
{
	public: 
		NPC(); // constructor

		void draw(sf::RenderWindow& window);
//		void startDialogue();
//		void update();

		sf::FloatRect getGlobalBounds() const;

	private:
		// constants
		const int NPC_SCALE = 6;
		const int NPC_HEIGHT = 48;
		const int NPC_WIDTH = 23;

		//sf::Texture texture;
		AnimatedCatSprite1 sprite;
		//bool showDialogue;

		// interaction text that displays when player interacts with the npc
		//sf::Text interactText;

		// text above cat so player knows to interact with npc
//		sf::Font font;
		sf::Text npcNametagText;

		//void dialogueSetup(int x_pos, int y_pos);


};
