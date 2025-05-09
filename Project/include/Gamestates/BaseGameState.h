#pragma once
#include "Literals.h"
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include <iostream>
#include <type_traits>
#include <cstring>

struct BaseGameState {
    using InitializeGamestateOperatorFunctionSignature = void (*)(void);
    using UpdateGamestateOperatorFunctionSignature = void (*)(void);
    using DrawGamestateOperatorFunctionSignature = void (*)(sf::RenderWindow&);
};

template <class Derived, class... INSTANCED_GAME_STATE_OPERATORS>
requires (sizeof...(INSTANCED_GAME_STATE_OPERATORS) > 0 && (std::is_base_of_v<BaseInstancedGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> && ...))
class InstancedGameState : public BaseGameState {
    // Workaround to make gamestate take stack space instead of heap space
    static inline constexpr u32 STACK_SIZE = (sizeof(INSTANCED_GAME_STATE_OPERATORS) + ...);
    uintptr_t initializationStackOffset;
    uintptr_t operatorStackPtrs[sizeof...(INSTANCED_GAME_STATE_OPERATORS)] = {};

    u8 OperatorStackSpace[STACK_SIZE];

    InstancedUpdatableGamestateOperator* updatables[(std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...) == 0 ? 1 : (std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...)];
    u32 updatablesIdx;

    template<class INSTANCED_GAME_STATE_OPERATOR>
    void PlacementNew(void) {
        BaseInstancedGamestateOperator* op = std::bit_cast<BaseInstancedGamestateOperator*>(new (std::bit_cast<void*>(std::bit_cast<uintptr_t>(&OperatorStackSpace[0]) + initializationStackOffset)) INSTANCED_GAME_STATE_OPERATOR);

        if constexpr(std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATOR>) {
            #ifdef DEBUG
            std::cout << "Registered updatable instanced gamestate operator " << typeid(INSTANCED_GAME_STATE_OPERATOR).name() << "\n";
            #endif
            updatables[updatablesIdx++] = std::bit_cast<INSTANCED_GAME_STATE_OPERATOR*>(op);
        }

        initializationStackOffset += sizeof(INSTANCED_GAME_STATE_OPERATOR);

        u32 emptyIdx;
        for(emptyIdx = 0; operatorStackPtrs[emptyIdx] != 0; emptyIdx++) ;

        operatorStackPtrs[emptyIdx] = std::bit_cast<uintptr_t>(op);
    }

    template<class INSTANCED_GAME_STATE_OPERATOR>
    requires(((std::is_same_v<INSTANCED_GAME_STATE_OPERATOR, INSTANCED_GAME_STATE_OPERATORS>) || ...))
    u32 GetOperatorStackPtrIdx(void) {
        bool foundIdx = false;
        s32 idx = -1;
        ((!foundIdx ? idx += std::is_same_v<INSTANCED_GAME_STATE_OPERATOR, INSTANCED_GAME_STATE_OPERATORS> : (s32) 0),...);

        return static_cast<u32>(idx);
    }

    protected:
    template<class INSTANCED_GAME_STATE_OPERATOR>
    requires(((std::is_same_v<INSTANCED_GAME_STATE_OPERATOR, INSTANCED_GAME_STATE_OPERATORS>) || ...))
    INSTANCED_GAME_STATE_OPERATOR& GetInstancedOperator(void) {
        u32 idx = GetOperatorStackPtrIdx<INSTANCED_GAME_STATE_OPERATOR>();

        return *std::bit_cast<INSTANCED_GAME_STATE_OPERATOR*>(operatorStackPtrs[idx]);
    }

    public:
    InstancedGameState() {
        initializationStackOffset = 0;
        updatablesIdx = 0;

        memset(updatables, 0, sizeof(updatables));

        // Construct the operators
        ((PlacementNew<INSTANCED_GAME_STATE_OPERATORS>(),...));
    }

    virtual void Update(void) = 0;
    virtual void Draw(sf::RenderWindow&) = 0;

    void __Update(void) {
        // First call the operator updates...
        for(InstancedUpdatableGamestateOperator* updatable : updatables) {
            if(updatable) {
                updatable->Update();
            }
        }

        // Then run the gamestate update
        static_cast<Derived*>(this)->Update();
    }

    void __Draw(sf::RenderWindow& window) {
        // Run the gamestate draw
        static_cast<Derived*>(this)->Draw(window);
    }

    virtual ~InstancedGameState() {
        u32 currentOperator = 0;
        ((std::bit_cast<INSTANCED_GAME_STATE_OPERATORS*>(operatorStackPtrs[currentOperator++])->~INSTANCED_GAME_STATE_OPERATORS()),...);
    }

    consteval std::size_t GetSize(void) {
        std::size_t size = 0;
        ((size += sizeof(INSTANCED_GAME_STATE_OPERATORS)),...);
        return size;
    }
};

template <class Derived, class... STATIC_GAME_STATE_OPERATORS>
requires (sizeof...(STATIC_GAME_STATE_OPERATORS) > 0 && (std::is_base_of_v<BaseStaticGamestateOperator, STATIC_GAME_STATE_OPERATORS> && ...))
class StaticGameState : public BaseGameState {
    static inline bool isInitialized = false;

    template<class STATIC_GAME_STATE_OPERATOR>
    static void CallOperatorInitialize(void) {
        if constexpr(std::derived_from<STATIC_GAME_STATE_OPERATOR, StaticInitializableGamestateOperator<STATIC_GAME_STATE_OPERATOR>>) {
            #ifdef DEBUG
            std::cout << "Initializing static gamestate operator " << typeid(STATIC_GAME_STATE_OPERATOR).name() << "\n";
            #endif
            STATIC_GAME_STATE_OPERATOR::Initialize();
        }
    }

    template<class STATIC_GAME_STATE_OPERATOR>
    static void CallOperatorUpdate(void) {
        if constexpr(std::derived_from<STATIC_GAME_STATE_OPERATOR, StaticUpdatableGamestateOperator<STATIC_GAME_STATE_OPERATOR>>)     
            STATIC_GAME_STATE_OPERATOR::Update();
    }

    protected:


    static void __Initialize(void) {

        if(!isInitialized) {
            // First call initialize for the individual operators...
            ((CallOperatorInitialize<STATIC_GAME_STATE_OPERATORS>(),...));

            // Then run initialize from the gamestate
            Derived::Initialize();
            isInitialized = true;
        }
    }

    static void __Update(void) {
        // First call update for the individual operators...
        ((CallOperatorUpdate<STATIC_GAME_STATE_OPERATORS>(),...));

        // Then run update from the gamestate
        Derived::Update();
    }

    static void __Draw(sf::RenderWindow& window) {
        // Call draw from the gamestate
        Derived::Draw(window);
    }

    public:

    static auto GetInitFunc(void) { return &__Initialize; }
    static auto GetUpdateFunc(void) { return &__Update; }
    static auto GetDrawFunc(void) { return &__Draw; }
};