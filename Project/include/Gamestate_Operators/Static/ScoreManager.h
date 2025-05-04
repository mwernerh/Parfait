#pragma once
#include "Literals.h" 
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Gamestate_Operators/BaseGameStateOperators.h"

class ScoreManager : public StaticInitializableGamestateOperator, public StaticDrawableGamestateOperator {
    struct ScoreWrapper {
        u32 score;
        sf::Text text;
    };

    /**
     * @brief Returns the ScoreWrapper singleton
     * @details score: Unsigned 32-bit integer that keeps track of the player's score.
     *          font: The font used to display the player's score.
     *          text: The text string that displays the player's score.
     * 
     * @return ScoreWrapper& 
     */
     static ScoreWrapper& GetScoreWrapper(void);

    public:

    /**
     * @brief Initializes the ScoreWrapper singleton. Should only be run once
     * @details score: Unsigned 32-bit integer that keeps track of the player's score. 
     *          font: The font used to display the player's score.
     *          text: The text string that displays the player's score.
     * 
     * @return ScoreWrapper& 
     */
    static void Initialize(void);

    /**
     * @brief Adds points to the player's score, accounting for potential overflow by capping at 0xFFFFFFFF
     * 
     * @param added_score 
     */
    static void AddScore(const u32 added_score);

    /**
     * @brief Subtracts points from the player's score, account for potential underflow by capping at 0
     * 
     * @param subtracted_score 
     */
    static void SubtractScore(const u32 subtracted_score);

    /**
     * @brief Gets the player's score
     * 
     * @return const u32& 
     */
    static const u32& GetScore(void);

    /**
     * @brief Sets the player's score to a specific value
     * 
     * @param new_score 
     */
    static void SetScore(const u32 new_score);

    /**
     * @brief Draws the text displaying the user's score
     * 
     * @param window 
     */
    static void Draw(sf::RenderWindow& window);
};