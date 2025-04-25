#pragma once
#include "SFML/Graphics/RectangleShape.hpp"

/**
 * @author Mar Werner Hernandez
 * @brief Hitboxes used by actors to attack and be damaged  (see also HitboxManager.h)
 */

/**
 * @brief Hitbox base class
 * 
 */
struct Hitbox : public sf::RectangleShape {
    bool isActive;

    Hitbox(bool isActive) : isActive(isActive) {}
};

struct AttackHitbox : public Hitbox {
    AttackHitbox() : Hitbox(false) {}
};

/**
 * @example
 * // Implementing a class that uses both attacking and self-damaging hitboxes.
 *
 * class MyEnemy {
 *      AttackHitbox at;
 *      ColliderHitbox co;
 *      sf::Sprite sprite;
 *      int health;
 *  
 *      public:
 *      
 *      // Constructor for enemy instance, providing collider hitbox with its parent (this instance) and the function that should be run when a hit is registered
 *      MyEnemy() : at(), co(this, std::bit_cast<ColliderHitbox::HitFuncGeneric>(&MyEnemy::MyHitFunction)) { health = 3; }
 *
 *      void Update() {
 *          HitboxManager::RegisterAttackHitbox(&at);
 *          HitboxManager::RegisterColliderHitbox(&co);
 *
 *          // User is responsible for keeping the hitbox and sprite movement aligned
 *          sprite.Move(...);
 *          co.Move(...);
 *          at.Move(...);
 *          ...
 *      }
 *  
 *      static void MyHitFunction(MyEnemy* const instance, const AttackHitbox* const attacker) {
 *          instance->health--;
 *          instance->Move(instance->GetPosition() - attacker->getPosition());
 *          ...
 *      }
 *  };
 * 
 */
class ColliderHitbox : public Hitbox {
    public:
    using HitFuncGeneric = void (*)(void* const, const AttackHitbox* const);

    private:
    void* parent;                   // The actor to which this hitbox is attached
    HitFuncGeneric onHitFunction;   // The function that runs on the collider intersecting with an active attack hitbox
    
    public:
    ColliderHitbox(void* parent, HitFuncGeneric on_hit_function) : Hitbox(true), parent(parent), onHitFunction(std::bit_cast<HitFuncGeneric>(on_hit_function)) {}
    void RunHitFunction(const AttackHitbox* const attacking_hitbox) { onHitFunction(parent, attacking_hitbox); }
};
