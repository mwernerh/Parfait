#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"

/*
 * @author Izzy Carlson
 * 
 * @brief Member variables and functions relating to the NPC. In this case, the NPC is on the title screen and will provide
 *  direction for controls to the user on the title screen page of the game.
 * 
 **/

class NPC
{
	public:
		// constructor to initialize NPC
		NPC();

		/**
		 * @brief draws npc and related text and sprites to the window
		 * 
		 * @param reference to the window to be drawn to
		 *
		 **/
		void draw(sf::RenderWindow& window);

		/**
		 * @brief enables dialogue to be drawn on the screen and plays audio reading aloud dialogue
		 **/
		void startDialogue();
		
		/**
		 * @brief gets NPC's global bounds
		 *
		 * @return float rect of global bounds
		 *
		 **/
		sf::FloatRect getGlobalBounds() const;

		/**
		 * @brief setter to determine dialogue visibility
		 * 
		 * @param boolean; true --> dialogue gets drawn to window false --> dialogue does not get drawn to window
		 *
		 **/
		void setShowDialogue(bool showDialogue);
		
		/**
		 * @brief setter to determine if dialogue audio should be played
		 *
		 * @param boolean; true -> dialogue sound gets played false -> dialogue sound does not get played
		 *
		 **/
		void setPlaySound(bool playSound);

	private:
		// constants for npc set up
		const int NPC_SCALE = 6;
		const int NPC_HEIGHT = 48;
		const int NPC_WIDTH = 23;
		
		AnimatedCatSprite1 sprite; // npc sprite
		sf::Texture texture; // texture for dialogue speech bubble
		sf::Sprite textSprite; // sprite for dialogue speech bubble
		bool showDialogue = false; // variable to determine whether dialogue should be drawn to the window
		bool playSound = true; // variable to determine whether dialogue audio should be played

		// interaction text that displays when player interacts with the npc
		sf::Text mControls; // text to hold controls for movement
		sf::Text aControls; // text to hold controls for attack & entry to the game

		// text above cat so player knows to interact with npc
		sf::Text npcNametagText;
};
