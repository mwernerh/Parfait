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
struct StaticInitializableGamestateOperator : public BaseStaticGamestateOperator {
    void Initialize(this auto&& derived) requires (decltype(derived)::Initialize()) {
        derived.Initialize();
    }
};

struct StaticUpdatableGamestateOperator : public BaseStaticGamestateOperator {
    void Update(this auto&& derived) requires (decltype(derived)::Update()) {
        derived.Update();
    }
};

struct StaticDrawableGamestateOperator : public BaseStaticGamestateOperator {
    void Draw(this auto&& derived, sf::RenderWindow& window) requires (decltype(derived)::Draw(window)) {
        derived.Draw(window);
    }
};