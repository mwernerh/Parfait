#include <SFML/Graphics.hpp>
#include <iostream>
#include "NPC.h"
#include "Framework_Managers/FontManager.h"
#include "Framework_Managers/AudioManager.h"

/** 
 *
 * @author Izzy Carlson
 *
 * Handles everything relating to the NPC and dialogue set-up.
 *
 **/

NPC::NPC()
{
	// set up NPC sprite
	sprite.SetAnimation<"Idle">();
	sprite.setPosition(200, 530); // bottom left corner
	sprite.setScale(NPC_SCALE, NPC_SCALE);

	// put text above cat so player knows they should interact with it
	npcNametagText.setFont(FontManager::GetFont("Akira Expanded Demo"));
	npcNametagText.setString("Cat Friend");
	npcNametagText.setCharacterSize(24);
	npcNametagText.setOrigin(npcNametagText.getLocalBounds().width / 2.0f, npcNametagText.getLocalBounds().height / 2.0f);
	npcNametagText.setFillColor(sf::Color::White);
	npcNametagText.setOutlineThickness(4.f);
	npcNametagText.setOutlineColor(sf::Color::Black);
	npcNametagText.setPosition(320, 480);

	// set-up text bubble sprite
	if (!texture.loadFromFile("./assets/txr/text_bubble/title_bubble.png"))
		std::cerr << "Error loading text bubble!" << std::endl;
	
	// set-up text bubble that will display text
	textSprite.setTexture(texture);
	textSprite.setPosition(120, 220);
	textSprite.setOrigin(0,0);
	textSprite.setScale(5, 5);

	// set-up dialogue for movement controls text
	mControls.setFont(FontManager::GetFont("Akira Expanded Demo"));
	mControls.setString("Press F to move left.\nPress J to move right.");
	mControls.setCharacterSize(24);
	mControls.setOrigin(mControls.getLocalBounds().width / 2.0f, mControls.getLocalBounds().height / 2.0f);
	mControls.setFillColor(sf::Color::White);
	mControls.setOutlineThickness(2.f);
	mControls.setOutlineColor(sf::Color::Black);
	mControls.setPosition(350, 310);
	
	// set-up dialogue for attack control and good luck message text
	aControls.setFont(FontManager::GetFont("Akira Expanded Demo"));
	aControls.setString("  Press K to attack.\nPress space to start.\n        Good luck!");
	aControls.setCharacterSize(24);
	aControls.setOrigin(aControls.getLocalBounds().width / 2.0f, aControls.getLocalBounds().height / 2.0f);
	aControls.setFillColor(sf::Color::White);
	aControls.setOutlineThickness(2.f);
	aControls.setOutlineColor(sf::Color::Black);
	aControls.setPosition(345, 377);
}

sf::FloatRect NPC::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

void NPC::setShowDialogue(bool showDialogue)
{
	this->showDialogue = showDialogue;
}

void NPC::setPlaySound(bool playSound)
{
	this->playSound = playSound;
}

void NPC::draw(sf::RenderWindow& window)
{
	sprite.Update();
	window.draw(sprite);
	window.draw(npcNametagText);

	if (showDialogue)
	{	
		window.draw(textSprite);
		window.draw(mControls);
		window.draw(aControls);
	}
	
}

void NPC::startDialogue()
{

	// dialogue appears as text on top of a speech bubble
	showDialogue = true;

	// play sound once to make sure audio doesn't overlay on top of each other	
	if (playSound){
		playSound = false;
		AudioManager::StartCameraSound("npc_ctrl");
	}
}
