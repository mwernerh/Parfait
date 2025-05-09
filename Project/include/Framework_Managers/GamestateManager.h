#pragma once
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "Literals.h"
#include "Gamestates/BaseGameState.h"
#include <iostream>

class GamestateManager {
    static inline constexpr u32 KIBIBYTE = 1024;
    static inline constexpr u32 MEBIBYTE = 1024 * KIBIBYTE;
    static inline constexpr u64 INSTANCED_STACK_SPACE_SIZE = 24 * MEBIBYTE;

    static inline bool isCurrentGamestateInstanced = false;

    static inline u8 instancedStackSpace[INSTANCED_STACK_SPACE_SIZE] = {};

    using InstancedUpdateFuncType = decltype(&InstancedGameState<BaseGameState, BaseInstancedGamestateOperator>::__Update);
    using InstancedDrawFuncType = decltype(&InstancedGameState<BaseGameState, BaseInstancedGamestateOperator>::__Draw);

    static inline InstancedUpdateFuncType instanceUpdate = nullptr;
    static inline InstancedDrawFuncType instanceDraw = nullptr;
    static inline void (*instanceDestructor)(void) = nullptr;

    static inline BaseGameState::InitializeGamestateOperatorFunctionSignature staticInit = nullptr;
    static inline BaseGameState::UpdateGamestateOperatorFunctionSignature staticUpdate = nullptr;
    static inline BaseGameState::DrawGamestateOperatorFunctionSignature staticDraw = nullptr;

    public:
    template<class STATIC_GAMESTATE>
    static void SwitchToStaticGamestate(void) {
        #ifdef DEBUG
        std::cout << "Switching to static gamestate " << typeid(STATIC_GAMESTATE).name() << "\n";
        #endif

        if(instanceDestructor != nullptr) {
            instanceDestructor();
            instanceDestructor = nullptr;
        }

        isCurrentGamestateInstanced = false;
        staticInit = STATIC_GAMESTATE::GetInitFunc();
        staticUpdate = STATIC_GAMESTATE::GetUpdateFunc();
        staticDraw = STATIC_GAMESTATE::GetDrawFunc();

        staticInit();
    }

    template<class INSTANCED_GAMESTATE>
    requires(sizeof(INSTANCED_GAMESTATE) <= INSTANCED_STACK_SPACE_SIZE)
    static void SwitchToInstancedGamestate(void) {
        #ifdef DEBUG
        std::cout << "Switching to instanced gamestate " << typeid(INSTANCED_GAMESTATE).name() << "\n";
        #endif

        if(instanceDestructor != nullptr) {
            instanceDestructor();
            instanceDestructor = nullptr;
        }

        isCurrentGamestateInstanced = true;
        new (instancedStackSpace) INSTANCED_GAMESTATE;

        instanceUpdate = std::bit_cast<InstancedUpdateFuncType>(&INSTANCED_GAMESTATE::__Update);
        instanceDraw = std::bit_cast<InstancedDrawFuncType>(&INSTANCED_GAMESTATE::__Draw);
        instanceDestructor = []() {
            std::bit_cast<const INSTANCED_GAMESTATE*>(&instancedStackSpace[0])->~INSTANCED_GAMESTATE();
        };
    }

    static void InitializeOperators(void) {
        staticInit();
    }

    static void UpdateOperators(void) {
        if(isCurrentGamestateInstanced) {
            (std::bit_cast<InstancedGameState<BaseGameState, BaseInstancedGamestateOperator>*>(&instancedStackSpace[0])->*instanceUpdate)();
        }
        else
            staticUpdate();
    }

    static void DrawOperators(sf::RenderWindow& window) {
        if(isCurrentGamestateInstanced) {
            (std::bit_cast<InstancedGameState<BaseGameState, BaseInstancedGamestateOperator>*>(&instancedStackSpace[0])->*instanceDraw)(window);
        }
        else
            staticDraw(window);
    }
};