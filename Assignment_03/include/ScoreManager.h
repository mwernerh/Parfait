#pragma once
#include "Literals.h" 

class ScoreManager {
    static u32 score;

    public:
    static void AddScore(const u32 added_score);
    static void SubtractScore(const u32 subtracted_score);
    static const u32& GetScore(void);
    static void SetScore(const u32 new_score);
};