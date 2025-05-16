#pragma once
#include "Literals.h"
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include <iostream>
#include <type_traits>
#include <cstring>

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Inherited from for compile-time checking 
 * 
 */
struct BaseGameState {
    using InitializeGamestateOperatorFunctionSignature = void (*)(void);
    using UpdateGamestateOperatorFunctionSignature = void (*)(void);
    using DrawGamestateOperatorFunctionSignature = void (*)(sf::RenderWindow&);
};

/**
 * @brief Inherited from to denote that a class is a gamestate that can be instantiated in stack space and destroyed
 * 
 * @tparam Derived - The class being defined as an instanced gamestate
 * @tparam INSTANCED_GAME_STATE_OPERATORS - The operators that should be associated with Derived
 */
template <class Derived, class... INSTANCED_GAME_STATE_OPERATORS>
requires (sizeof...(INSTANCED_GAME_STATE_OPERATORS) > 0 && (std::is_base_of_v<BaseInstancedGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> && ...))
class InstancedGameState : public BaseGameState {
    // Workaround to make gamestate take stack space instead of heap space
    static inline constexpr u32 STACK_SIZE = (sizeof(INSTANCED_GAME_STATE_OPERATORS) + ...);
    uintptr_t initializationStackOffset;
    uintptr_t operatorStackPtrs[sizeof...(INSTANCED_GAME_STATE_OPERATORS)] = {};    // Pointers to each of the operators within the stack space

    u8 OperatorStackSpace[STACK_SIZE];  // Stack space allocated for the operators

    InstancedUpdatableGamestateOperator* updatables[(std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...) == 0 ? 1 : (std::is_base_of_v<InstancedUpdatableGamestateOperator, INSTANCED_GAME_STATE_OPERATORS> + ...)];
    u32 updatablesIdx;

    /**
     * @brief Constructs an instanced gamestate operator in the provided stack space, increments the offset into the stack for future constructor calls for the other operators
     * 
     * @tparam INSTANCED_GAME_STATE_OPERATOR 
     */
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

    /**
     * @brief Obtains the index into the array of pointers to the stack space of the operators, for the desired operator type
     * 
     * @tparam INSTANCED_GAME_STATE_OPERATOR - The operator whose position in the stack is being searched for
     * @return u32 
     */
    template<class INSTANCED_GAME_STATE_OPERATOR>
    requires(((std::is_same_v<INSTANCED_GAME_STATE_OPERATOR, INSTANCED_GAME_STATE_OPERATORS>) || ...))
    u32 GetOperatorStackPtrIdx(void) {
        bool foundIdx = false;
        s32 idx = -1;
        ((!foundIdx ? idx += std::is_same_v<INSTANCED_GAME_STATE_OPERATOR, INSTANCED_GAME_STATE_OPERATORS> : (s32) 0),...);

        return static_cast<u32>(idx);
    }

    protected:
    /**
     * @brief Obtains a reference to the instance of the desired operator, with its correct offset in the provided stack space
     * 
     * @tparam INSTANCED_GAME_STATE_OPERATOR
     * @return A reference to an instanced gamestate operator
     */
    template<class INSTANCED_GAME_STATE_OPERATOR>
    requires(((std::is_same_v<INSTANCED_GAME_STATE_OPERATOR, INSTANCED_GAME_STATE_OPERATORS>) || ...))
    INSTANCED_GAME_STATE_OPERATOR& GetInstancedOperator(void) {
        u32 idx = GetOperatorStackPtrIdx<INSTANCED_GAME_STATE_OPERATOR>();

        return *std::bit_cast<INSTANCED_GAME_STATE_OPERATOR*>(operatorStackPtrs[idx]);
    }

    public:
    /**
     * @brief Construct a new InstancedGameState and its operators
     * 
     */
    InstancedGameState() {
        initializationStackOffset = 0;
        updatablesIdx = 0;

        memset(updatables, 0, sizeof(updatables));

        // Construct the operators
        ((PlacementNew<INSTANCED_GAME_STATE_OPERATORS>(),...));
    }

    virtual void Update(void) = 0;
    virtual void Draw(sf::RenderWindow&) = 0;

    /**
     * @brief First calls all of the update functions of the operators (in the order in which they were provided) then calls the update function of the gamestate itself
     * 
     */
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

    /**
     * @brief Calls the draw function of the gamestate itself, which should specify the proper draw order of the operators
     * 
     */
    void __Draw(sf::RenderWindow& window) {
        // Run the gamestate draw
        static_cast<Derived*>(this)->Draw(window);
    }

    /**
     * @brief Destroy the InstancedGameState and its associated operators
     * 
     */
    virtual ~InstancedGameState() {
        u32 currentOperator = 0;
        ((std::bit_cast<INSTANCED_GAME_STATE_OPERATORS*>(operatorStackPtrs[currentOperator++])->~INSTANCED_GAME_STATE_OPERATORS()),...);
    }

    /**
     * @brief Gets the space taken up by the operators
     */
    consteval std::size_t GetSize(void) {
        std::size_t size = 0;
        ((size += sizeof(INSTANCED_GAME_STATE_OPERATORS)),...);
        return size;
    }
};

/**
 * @brief Inherited from to denote that a class is a static gamestate that will be present throughout program execution
 * 
 * @tparam Derived - The class being defined as a static gamestate
 * @tparam STATIC_GAME_STATE_OPERATORS - The operators that should be associated with Derived
 */
template <class Derived, class... STATIC_GAME_STATE_OPERATORS>
requires (sizeof...(STATIC_GAME_STATE_OPERATORS) > 0 && (std::is_base_of_v<BaseStaticGamestateOperator, STATIC_GAME_STATE_OPERATORS> && ...))
class StaticGameState : public BaseGameState {
    static inline bool isInitialized = false;   // Used to determine which gamestate should be run -- the static one currently stored, or the instanced one currently stored

    /**
     * @brief Initializes the specified operator, ensuring it is initializable
     * 
     * @tparam STATIC_GAME_STATE_OPERATOR 
     */
    template<class STATIC_GAME_STATE_OPERATOR>
    static void CallOperatorInitialize(void) {
        if constexpr(std::derived_from<STATIC_GAME_STATE_OPERATOR, StaticInitializableGamestateOperator<STATIC_GAME_STATE_OPERATOR>>) {
            #ifdef DEBUG
            std::cout << "Initializing static gamestate operator " << typeid(STATIC_GAME_STATE_OPERATOR).name() << "\n";
            #endif
            STATIC_GAME_STATE_OPERATOR::Initialize();
        }
    }

    /**
     * @brief Updates the specified operator, ensuring it is updatable
     * 
     * @tparam STATIC_GAME_STATE_OPERATOR 
     */
    template<class STATIC_GAME_STATE_OPERATOR>
    static void CallOperatorUpdate(void) {
        if constexpr(std::derived_from<STATIC_GAME_STATE_OPERATOR, StaticUpdatableGamestateOperator<STATIC_GAME_STATE_OPERATOR>>)     
            STATIC_GAME_STATE_OPERATOR::Update();
    }

    protected:


    /**
     * @brief Initializes all of the initializable operators, then initializes the static gamestate itself
     * 
     */
    static void __Initialize(void) {

        if(!isInitialized) {
            // First call initialize for the individual operators...
            ((CallOperatorInitialize<STATIC_GAME_STATE_OPERATORS>(),...));

            // Then run initialize from the gamestate
            Derived::Initialize();
            isInitialized = true;
        }
    }

    /**
     * @brief Updates all of the initializable operators, then updates the static gamestate itself
     * 
     */
    static void __Update(void) {
        // First call update for the individual operators...
        ((CallOperatorUpdate<STATIC_GAME_STATE_OPERATORS>(),...));

        // Then run update from the gamestate
        Derived::Update();
    }

    /**
     * @brief Draws the static gamestate, which should specify the draw order of its operators
     * 
     */
    static void __Draw(sf::RenderWindow& window) {
        // Call draw from the gamestate
        Derived::Draw(window);
    }

    public:

    /**
     * @brief Obtains a pointer to function that will first initialize the operators, then the static gamestate itself
     * 
     * @return PTMF
     */
    static auto GetInitFunc(void) { return &__Initialize; }

    /**
     * @brief Obtains a pointer to function that will first update the operators, then the static gamestate itself
     * 
     * @return PTMF
     */
    static auto GetUpdateFunc(void) { return &__Update; }

    /**
     * @brief Obtains a pointer to function that will draw the static gamestate
     * 
     * @return PTMF
     */
    static auto GetDrawFunc(void) { return &__Draw; }
};