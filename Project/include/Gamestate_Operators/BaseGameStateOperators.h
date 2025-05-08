#pragma once
#include "SFML/Graphics/RenderWindow.hpp"

struct BaseInstancedGamestateOperator {};

struct InstancedUpdatableGamestateOperator : public BaseInstancedGamestateOperator {
    virtual void Update(void) = 0;
};

struct InstancedDrawableGamestateOperator : public BaseInstancedGamestateOperator {
    virtual void Draw(sf::RenderWindow&) = 0;
};


struct BaseStaticGamestateOperator {};

// Use CRTP to enforce Initialize, Update, and Draw function presence
template <class Derived>
struct StaticInitializableGamestateOperator : public BaseStaticGamestateOperator {
    void Initialize() requires (Derived::Initialize()) {
        Derived::Initialize();
    }
};

template <class Derived>
struct StaticUpdatableGamestateOperator : public BaseStaticGamestateOperator {
    void Update() requires (Derived::Update()) {
        Derived::Update();
    }
};

template <class Derived>
struct StaticDrawableGamestateOperator : public BaseStaticGamestateOperator {
    void Draw(sf::RenderWindow& window) requires(Derived::Draw(window)) {
        Derived::Draw(window);
    }
};