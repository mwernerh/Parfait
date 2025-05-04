#pragma once
#include "Literals.h"
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include <type_traits>

struct BaseGameState {
    using InitializeGamestateOperatorFunctionSignature = void (*)(void);
    using UpdateGamestateOperatorFunctionSignature = void (*)(void);
    using DrawGamestateOperatorFunctionSignature = void (*)(sf::RenderWindow&);
};

template <class... INSTANCED_GAME_STATE_OPERATORS>
requires (sizeof...(INSTANCED_GAME_STATE_OPERATORS) > 0 && (std::is_base_of_v<BaseInstancedGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> && ...))
class InstancedGameState : public BaseGameState {
    // Workaround to make gamestate take stack space instead of heap space
    static inline constexpr u32 STACK_SIZE = (sizeof(INSTANCED_GAME_STATE_OPERATORS) + ...);
    u8 OperatorStackSpace[STACK_SIZE];

    InstancedUpdatableGamestateOperator* updatables[(std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...) == 0 ? 1 : (std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...)];
    InstancedDrawableGamestateOperator* drawables[(std::is_base_of_v<InstancedDrawableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...) == 0 ? 1 : (std::is_base_of_v<InstancedDrawableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...)];

    public:
    InstancedGameState();
    virtual void RunUpdatables(void) = 0;
    virtual void RunDrawables(void) = 0;
};

template <class Derived, class... STATIC_GAME_STATE_OPERATORS>
requires (sizeof...(STATIC_GAME_STATE_OPERATORS) > 0 && (std::is_base_of_v<BaseStaticGamestateOperator, STATIC_GAME_STATE_OPERATORS> && ...))
class StaticGameState : public BaseGameState {
    /*
    protected:

    static inline InitializeGamestateOperatorFunctionSignature initializables[(std::is_base_of<StaticInitializableGamestateOperator, STATIC_GAME_STATE_OPERATORS>::value + ...) == 0 ? 1 : (std::is_base_of<StaticInitializableGamestateOperator, STATIC_GAME_STATE_OPERATORS>::value + ...)] = 
    static inline UpdateGamestateOperatorFunctionSignature updatables[(std::is_base_of<StaticUpdatableGamestateOperator, STATIC_GAME_STATE_OPERATORS>::value + ...) == 0 ? 1 : (std::is_base_of<StaticUpdatableGamestateOperator, STATIC_GAME_STATE_OPERATORS>::value + ...)] = 
                    {(&STATIC_GAME_STATE_OPERATORS::Update)...};
    static inline DrawGamestateOperatorFunctionSignature drawables[(std::is_base_of<StaticDrawableGamestateOperator, STATIC_GAME_STATE_OPERATORS>::value + ...) == 0 ? 1 : (std::is_base_of<StaticDrawableGamestateOperator, STATIC_GAME_STATE_OPERATORS>::value + ...)] =
                    {(&STATIC_GAME_STATE_OPERATORS::Draw)...};
    */

    static inline bool isInitialized = false;

    template<class STATIC_GAME_STATE_OPERATOR>
    static void CallOperatorInitialize(void) {
        if constexpr(std::derived_from<STATIC_GAME_STATE_OPERATOR, StaticInitializableGamestateOperator>) {
            STATIC_GAME_STATE_OPERATOR::Initialize();
        }
    }

    template<class STATIC_GAME_STATE_OPERATOR>
    static void CallOperatorUpdate(void) {
        if constexpr(std::derived_from<STATIC_GAME_STATE_OPERATOR, StaticUpdatableGamestateOperator>) {
            STATIC_GAME_STATE_OPERATOR::Update();
        }
    }

    public:

    static auto GetInitFunc(void) { return &__Initialize; }
    static auto GetUpdateFunc(void) { return &__Update; }
    static auto GetDrawFunc(void) { return &__Draw; }

    //using StaticGameStateBase = StaticGameState<Derived, STATIC_GAME_STATE_OPERATORS...>;

    protected:


    static void __Initialize(void) {

        if(!isInitialized) {
            //((std::derived_from<STATIC_GAME_STATE_OPERATORS, StaticInitializableGamestateOperator> ? STATIC_GAME_STATE_OPERATORS::StaticGameStateBase::Initialize() : (void) 0),...);

            ((CallOperatorInitialize<STATIC_GAME_STATE_OPERATORS>(),...));

            Derived::Initialize();
            isInitialized = true;
        }
    }

    static void __Update(void) {
        //((std::is_base_of_v<StaticUpdatableGamestateOperator, STATIC_GAME_STATE_OPERATORS> ? STATIC_GAME_STATE_OPERATORS::StaticGameStateBase::Update() : (void) 0),...);

        ((CallOperatorUpdate<STATIC_GAME_STATE_OPERATORS>(),...));

        Derived::Update();
    }

    static void __Draw(sf::RenderWindow& window) {
        Derived::Draw(window);
    }
};