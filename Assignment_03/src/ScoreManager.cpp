#include "ScoreManager.h"
#include <iostream>

/**
 * @author Mar Werner Hernandez
 */

void ScoreManager::Initialize(void) {
    ScoreWrapper& scoreWrapper = GetScoreWrapper();

    if(!scoreWrapper.font.loadFromFile("./assets/fnt/Akira Expanded Demo.otf"))
        std::cerr << "\x1b[31mError! Could not load font for score!\x1b[0m\n";

    scoreWrapper.text.setFont(scoreWrapper.font);
    scoreWrapper.text.setString("Score ");
    scoreWrapper.text.setCharacterSize(24);
    scoreWrapper.text.setFillColor(sf::Color::White);
    scoreWrapper.text.setOutlineThickness(5.0f);
    scoreWrapper.text.setOutlineColor(sf::Color::Black);
    scoreWrapper.score = 0;
}

ScoreManager::ScoreWrapper& ScoreManager::GetScoreWrapper(void) {
    static ScoreWrapper score;
    return score;
}

void ScoreManager::AddScore(const u32 added_score) {
    ScoreWrapper& scoreWrapper = GetScoreWrapper();

    [[likely]]
    if(scoreWrapper.score < static_cast<decltype(scoreWrapper.score)>(scoreWrapper.score + added_score))
        scoreWrapper.score += added_score;
    else
        scoreWrapper.score = -1;
}

void ScoreManager::SubtractScore(const u32 subtracted_score) {
    ScoreWrapper& scoreWrapper = GetScoreWrapper();

    [[likely]]
    if(scoreWrapper.score > static_cast<decltype(scoreWrapper.score)>(scoreWrapper.score - subtracted_score))
        scoreWrapper.score += subtracted_score;
    else
        scoreWrapper.score = 0;
}

const u32& ScoreManager::GetScore(void) {
    ScoreWrapper& scoreWrapper = GetScoreWrapper();
    return scoreWrapper.score;
}

void ScoreManager::SetScore(const u32 new_score) {
    ScoreWrapper& scoreWrapper = GetScoreWrapper();
    scoreWrapper.score = new_score;
}

void ScoreManager::Draw(sf::RenderWindow& window) {
    ScoreWrapper& scoreWrapper = GetScoreWrapper();
    
    scoreWrapper.text.setString(scoreWrapper.text.getString() + std::to_string(scoreWrapper.score));
    scoreWrapper.text.setOrigin(scoreWrapper.text.getLocalBounds().width / 2.0f, scoreWrapper.text.getLocalBounds().height / 2.0f);
    scoreWrapper.text.setPosition(window.getView().getCenter().x, window.getView().getCenter().y - (window.getView().getSize().y / 2.0f) + (scoreWrapper.text.getCharacterSize() / 2.0f));
    window.draw(scoreWrapper.text);
    scoreWrapper.text.setString("Score ");
}
