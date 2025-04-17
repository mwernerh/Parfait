#include "ScoreManager.h"

u32 ScoreManager::score = 0;

void ScoreManager::AddScore(const u32 added_score) {
    [[likely]]
    if(score < static_cast<decltype(score)>(score + added_score))
        score += added_score;
    else
        score = -1;
}

void ScoreManager::SubtractScore(const u32 subtracted_score) {
    [[likely]]
    if(score > static_cast<decltype(score)>(score - subtracted_score))
        score += subtracted_score;
    else
        score = 0;
}

const u32& ScoreManager::GetScore(void) {
    return score;
}

void ScoreManager::SetScore(const u32 new_score) {
    score = new_score;
}