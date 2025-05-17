# Controls
* Press F to move left
* Press J to move right
* Press K to attack
* Space to start game (Title screen)
* Space to pause (Main game state)
* K to save (When in pausing state)

# General Information
* The player has 100 health and health cannot be regenerated
* There are three levels, and each has two enemies each
* Level changes once the player has reached a score of 1000

# Title Screen
* Displays ginger cat NPC
* When the player gets close to the NPC, a chat bubble will appear that uses text to tell the player the controls and also plays an text-to-speech audio file that reads the controls

# Main Game State
* Three Levels
     * City
          * Rat enemy → Poisons the player to incrementally damage them
          * Bird enemy → Swoops down to attack the player
     * Park (further from home)
          * Bird enemy → See above
          * Yellow dog → Fast, less health, barking attack
     * Park (closer to home)
          * Bird enemy → See above
          * Blue dog → Slower, more health, barking attack

# Game Over
* The player dies if health reaches zero
* A screen will appear and the player can press space to restart from the beginning

# Ending Screen
* Ending cutscene that shows the player arriving home
