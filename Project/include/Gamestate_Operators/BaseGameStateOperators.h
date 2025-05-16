#pragma once
#include "SFML/Graphics/RenderWindow.hpp"

/**
 * @author Mar Werner Hernandez 
 * 
 */

// Used for polymorphism checks
struct BaseInstancedGamestateOperator {};

struct InstancedUpdatableGamestateOperator : public BaseInstancedGamestateOperator {
    virtual void Update(void) = 0;
};

struct InstancedDrawableGamestateOperator : public BaseInstancedGamestateOperator {
    virtual void Draw(sf::RenderWindow&) = 0;
};

// Used for compile-time polymorphism checks (CRTP)
struct BaseStaticGamestateOperator {};

// Use CRTP to enforce Initialize, Update, and Draw function presence

/**
 * @brief Inherited from to denote a class as an initializable operator for a static gamestate
 * 
 * @tparam The operator which is being tagged as an initializable operator
 */
template <class Derived>
struct StaticInitializableGamestateOperator : public BaseStaticGamestateOperator {
    void Initialize() requires (Derived::Initialize()) {
        Derived::Initialize();
    }
};

/**
 * @brief Inherited from to denote a class as an updatable operator for a static gamestate
 * 
 * @tparam The operator which is being tagged as an updatable operator
 */
template <class Derived>
struct StaticUpdatableGamestateOperator : public BaseStaticGamestateOperator {
    void Update() requires (Derived::Update()) {
        Derived::Update();
    }
};

/**
 * @brief Inherited from to denote a class as a drawable operator for a static gamestate
 * 
 * @tparam The operator which is being tagged as an drawable operator
 */
template <class Derived>
struct StaticDrawableGamestateOperator : public BaseStaticGamestateOperator {
    void Draw(sf::RenderWindow& window) requires(Derived::Draw(window)) {
        Derived::Draw(window);
    }
};