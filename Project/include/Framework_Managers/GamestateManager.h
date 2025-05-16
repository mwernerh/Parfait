#pragma once
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "Literals.h"
#include "Gamestates/BaseGameState.h"
#include <iostream>

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Interface to switch between various gamestates, so that only code from the current gamestate is run in the current frame
 * 
 */
class GamestateManager {
    static inline constexpr u32 KIBIBYTE = 1024;
    static inline constexpr u32 MEBIBYTE = 1024 * KIBIBYTE;
    static inline constexpr u64 INSTANCED_STACK_SPACE_SIZE = 24 * MEBIBYTE; // The stack space allocated to instanced gamestates

    static inline bool isCurrentGamestateInstanced = false; // Used to determine whether the static or instanced functions should be run

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
    /**
     * @brief Obtains the initialization, update, and draw functions of the desired gamestate and stores them
     * 
     * @tparam STATIC_GAMESTATE 
     */
    template<class STATIC_GAMESTATE>
    static void SwitchToStaticGamestate(void) {
        #ifdef DEBUG
        std::cout << "Switching to static gamestate " << typeid(STATIC_GAMESTATE).name() << "\n";
        #endif

        // If the previous gamestate was an instance, destroy it
        if(instanceDestructor != nullptr) {
            // Clang automatically generates the destructor -- g++ does not
            // Without this compile-time check for compiler used, the destructor may end up being called twice and spit out errors about freeing memory that was never allocated
            #ifndef __clang__
            instanceDestructor();
            #endif
            instanceDestructor = nullptr;
        }

        isCurrentGamestateInstanced = false;
        staticInit = STATIC_GAMESTATE::GetInitFunc();
        staticUpdate = STATIC_GAMESTATE::GetUpdateFunc();
        staticDraw = STATIC_GAMESTATE::GetDrawFunc();

        staticInit();
    }

    /**
     * @brief Obtains the constructor, update, and draw functions of the desired gamestate and its operators, and stores them
     * 
     * @tparam INSTANCED_GAMESTATE 
     */
    template<class INSTANCED_GAMESTATE>
    requires(sizeof(INSTANCED_GAMESTATE) <= INSTANCED_STACK_SPACE_SIZE)
    static void SwitchToInstancedGamestate(void) {
        #ifdef DEBUG
        std::cout << "Switching to instanced gamestate " << typeid(INSTANCED_GAMESTATE).name() << "\n";
        #endif

        // If the previous gamestate was an instance, destroy it
        if(instanceDestructor != nullptr) {
            // Clang automatically generates the destructor -- g++ does not
            // Without this compile-time check for compiler used, the destructor may end up being called twice and spit out errors about freeing memory that was never allocated
            #ifndef __clang__
            instanceDestructor();
            #endif
            instanceDestructor = nullptr;
        }

        isCurrentGamestateInstanced = true;
        new (instancedStackSpace) INSTANCED_GAMESTATE; // Placement new into the allocated stack space -- calls constructor as well

        instanceUpdate = std::bit_cast<InstancedUpdateFuncType>(&INSTANCED_GAMESTATE::__Update);
        instanceDraw = std::bit_cast<InstancedDrawFuncType>(&INSTANCED_GAMESTATE::__Draw);

        // PTMFs for destructors cannot be obtained -- use lambdas instead to encapsulate the functionality
        instanceDestructor = []() {
            std::bit_cast<const INSTANCED_GAMESTATE*>(&instancedStackSpace[0])->~INSTANCED_GAMESTATE();
        };
    }

    /**
     * @brief Initializes the operators of a static gamestate
     * 
     */
    static void InitializeOperators(void) {
        staticInit();
    }

    /**
     * @brief Update the operators composing the current state, as well as the gamestate itself
     * 
     */
    static void UpdateOperators(void) {
        if(isCurrentGamestateInstanced) {
            (std::bit_cast<InstancedGameState<BaseGameState, BaseInstancedGamestateOperator>*>(&instancedStackSpace[0])->*instanceUpdate)();
        }
        else
            staticUpdate();
    }

    /**
     * @brief Draw the operators in the order defined by the gamestate
     * 
     * @param window 
     */
    static void DrawOperators(sf::RenderWindow& window) {
        if(isCurrentGamestateInstanced) {
            (std::bit_cast<InstancedGameState<BaseGameState, BaseInstancedGamestateOperator>*>(&instancedStackSpace[0])->*instanceDraw)(window);
        }
        else
            staticDraw(window);
    }
};