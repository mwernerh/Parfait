#pragma once
#include "Gamestate_Operators/Static/MapManager.h"
#include "Gamestate_Operators/Static/HitboxManager.h"
#include "Gamestate_Operators/Static/LevelManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Gamestate_Operators/Static/ScoreManager.h"
#include "Gamestates/BaseGameState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Listener.hpp>

struct PlayMainGameState : public StaticGameState<PlayMainGameState, PlayerManager, HitboxManager, MapManager, LevelManager, ScoreManager> {
    static void Initialize(void);
    static void Update(void);
    static void Draw(sf::RenderWindow& window);
};