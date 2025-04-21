#pragma once
#include "Literals.h" 
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class ScoreManager {
    struct ScoreWrapper {
        u32 score;
        sf::Font font;
        sf::Text text;
    };

    public:
    static void Initialize(void);
    static ScoreWrapper& GetScoreWrapper(void);
    static void AddScore(const u32 added_score);
    static void SubtractScore(const u32 subtracted_score);
    static const u32& GetScore(void);
    static void SetScore(const u32 new_score);
    static void Draw(sf::RenderWindow& window);
};