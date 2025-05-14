#include <SFML/Graphics.hpp>
#include <iostream>
#include "NPC.h"
#include "Framework_Managers/FontManager.h"

NPC::NPC()
{	
// set up sprite
	sprite.SetAnimation<"Idle">();
	sprite.setPosition(200, 530); // bottom left corner (MAY NEED TO BE UPDATED);
	sprite.setScale(NPC_SCALE, NPC_SCALE);
//	sprite.Update();
	// put text above cat so player knows they should interact with it
	
/*    	if (!font.loadFromFile("/assets/fnt/Akira Expanded Demo.otf")) {
        	std::cerr << "Error loading font" << std::endl;
    	}*/

	
	npcNametagText.setFont(FontManager::GetFont("Akira Expanded Demo"));
	npcNametagText.setString("Cat Friend");
	npcNametagText.setCharacterSize(24);
	npcNametagText.setOrigin(npcNametagText.getLocalBounds().width / 2.0f, npcNametagText.getLocalBounds().height / 2.0f);
	npcNametagText.setFillColor(sf::Color::White);
	npcNametagText.setOutlineThickness(4.f);
	npcNametagText.setOutlineColor(sf::Color::Black);
	npcNametagText.setPosition(320, 480);
//	npcNametagText.setPosition(1280.0f / 2.0f, 720.0f / 2.0f);
	

	// set-up dialogue
//	dialogueSetup(x_pos, y_pos);

	// dialogue should not initially show 
//	showDialogue = false;

}

/*sf::FloatRect NPC::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}*/

void NPC::draw(sf::RenderWindow& window)
{
	sprite.Update();
	window.draw(sprite);
	window.draw(npcNametagText);

//	if (showDialogue)
//	{
//		window.draw(interactText);
//	}
}

/*void NPC::startDialogue()
{

	// dialogue appears as text
//	showDialogue = true;

	// play dialogue audio	


}*/

/*void NPC::dialogueSetup(int x_pos, int y_pos)
{
	interactText.setFont(FontManager::GetFont("Akira Expanded Demo"));
	interactText.setString("Press F to move left. J to move right. K to attack! Good luck!");
	interactText.setCharacterSize(12);
	interactText.setFillColor(sf::Color::White);
	interactText.setOutlineThickness(2.f);
	interactText.setOutlineColor(sf::Color::Black);
	interactText.setPosition(x_pos, y_pos + 24); // make sure above sprite + name (may need to be adjusted)
}*/
