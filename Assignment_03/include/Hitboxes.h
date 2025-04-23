#pragma once
#include "SFML/Graphics/RectangleShape.hpp"

struct Hitbox : public sf::RectangleShape {
    bool isActive;

    Hitbox() : isActive(false) {}
};

struct AttackHitbox : public Hitbox {};

class ColliderHitbox : public Hitbox {
    public:
    using HitFuncGeneric = void (*)(void* const, const AttackHitbox* const);

    private:
    void* parent;
    HitFuncGeneric onHitFunction;
    
    public:
    ColliderHitbox(void* parent, HitFuncGeneric on_hit_function) : Hitbox(), parent(parent), onHitFunction(std::bit_cast<HitFuncGeneric>(on_hit_function)) {}
    void RunHitFunction(AttackHitbox* attacking_hitbox) { onHitFunction(parent, attacking_hitbox); }
};