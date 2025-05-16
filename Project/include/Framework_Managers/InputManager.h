#pragma once
#include <SFML/Window.hpp>
#include <bitset>

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Provides access to current user input, and the frame delta time
 * @note This is NOT a gamestate operator, its update function MUST be run before that of any gamestate operator
 * 
 */
class InputManager {
    static std::bitset<sf::Keyboard::Scancode::ScancodeCount> states[2];
    static bool prevStateIdx;
    static float deltaTimeSeconds;

    static sf::Clock& GetDeltaTimeClock(void);

    static void IncrementState(void);
    static void ResetDeltaTimeClock(void);

    public:
    /**
     * @brief Initializes the clock used to obtain frame delta time
     * 
     */
    static void Initialize(void);

    /**
     * @brief Updates the frame delta time and sets the current input state as the previous input state
     * 
     */
    static void Update(void);

    /**
     * @brief Register that a key was pressed on the current frame
     * 
     * @param key 
     */
    static void RegisterKeyPress(sf::Keyboard::Scancode key);

    /**
     * @brief Register that a key was released on the current frame
     * 
     * @param key 
     */
    static void RegisterKeyRelease(sf::Keyboard::Scancode key);

    /**
     * @brief Checks whether a key has been initially pressed
     * 
     * @param key 
     * @return true when the key is first pressed
     * @return false if the key is being held, released, or just isn't pressed at all
     */
    static bool IsKeyPressed(sf::Keyboard::Scancode key);

    /**
     * @brief Checks whether a key is being held
     * 
     * @param key 
     * @return true when the key was pressed in the previous input state and is still being pressed in the current input state
     * @return false when the key is first pressed, is released, or just isn't pressed at all
     */
    static bool IsKeyHeld(sf::Keyboard::Scancode key);

    /**
     * @brief Checks whether a key is being released
     * 
     * @param key 
     * @return true when the key was just released in the current frame
     * @return false when the key is first pressed, is being held, or just isn't pressed at all
     */
    static bool IsKeyReleased(sf::Keyboard::Scancode key);

    /**
     * @brief Get the delta time for the current frame, in seconds
     * 
     * @return float 
     */
    static float GetDeltaTime(void);
};