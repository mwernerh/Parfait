#pragma once
#include "Literals.h"
#include "Gamestates/BaseGameState.h"

struct __DTOR {
    ~__DTOR(void);
};

class GamestateManager {
    static inline constexpr u32 KIBIBYTE = 1024;
    static inline constexpr u32 MEBIBYTE = 1024 * KIBIBYTE;
    static inline constexpr u64 INSTANCED_STACK_SPACE_SIZE = 24 * MEBIBYTE;

    static inline bool isCurrentGamestateInstanced = false;

    static inline u8 instancedStackSpace[INSTANCED_STACK_SPACE_SIZE] = {};


    static inline BaseGameState::InitializeGamestateOperatorFunctionSignature staticInit = nullptr;
    static inline BaseGameState::UpdateGamestateOperatorFunctionSignature staticUpdate = nullptr;
    static inline BaseGameState::DrawGamestateOperatorFunctionSignature staticDraw = nullptr;

    public:
    template<class STATIC_GAMESTATE>
    static void SwitchToStaticGamestate(void) {
        isCurrentGamestateInstanced = false;
        staticInit = STATIC_GAMESTATE::GetInitFunc();
        staticUpdate = STATIC_GAMESTATE::GetUpdateFunc();
        staticDraw = STATIC_GAMESTATE::GetDrawFunc();
    }

    template<class INSTANCED_GAMESTATE>
    static void SwitchToInstancedGamestate(void) {
        isCurrentGamestateInstanced = true;
        new (&instancedStackSpace[0]) INSTANCED_GAMESTATE;
    }

    static void InitializeOperators(void) {
        staticInit();
    }

    static void UpdateOperators(void) {
        staticUpdate();
    }

    static void DrawOperators(sf::RenderWindow& window) {
        staticDraw(window);
    }
};