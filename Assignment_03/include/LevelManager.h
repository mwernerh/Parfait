#pragma once
#include "Literals.h"

class LevelManager {
    static u32 currentLevel;

    public:

    /**
     * @brief Checks whether the level should change based on the player's score
     * 
     */
    static void Update(void);
};