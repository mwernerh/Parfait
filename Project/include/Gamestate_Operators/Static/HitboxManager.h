#pragma once
#include "Gamestate_Operators/BaseGameStateOperators.h"
#include "Literals.h"
#include "Hitboxes.h"

/**
 * @author Mar Werner Hernandez 
 * @brief System used to manage collisions between hitboxes
 * 
 */
class Player;

class HitboxManager : public StaticUpdatableGamestateOperator {
    static inline constexpr f32 ENEMY_APPROACH_SFX_TIMER_MAX = 0.3f;

    static constexpr u32 MAX_HITBOXES = 256;

    static AttackHitbox* enemyAtHitboxes[MAX_HITBOXES];
    static ColliderHitbox* enemyCoHitboxes[MAX_HITBOXES];

    static u32 numEnemyAtHitboxes;
    static u32 numEnemyCoHitboxes;

    static f32 enemyApproachSFXTimer;

    public:

    /**
     * @brief Registers an attacking hitbox to check later (see Update()) whether it intersects with a collider hitbox. DOES NOT MODIFY THE HITBOX!
     * 
     * @param attack_hitbox 
     */
    static void RegisterAttackHitbox(AttackHitbox* const attack_hitbox);

    /**
     * @brief Registers a colliding hitbox to check later (see Update()) whether it intersects with an attacking hitbox. DOES NOT MODIFY THE HITBOX!
     * 
     * @param collider_hitbox 
     */
    static void RegisterColliderHitbox(ColliderHitbox* const collider_hitbox);

    /**
     * @brief Iterates through the registered hitboxes and checks for intersections, then clears the list of registered hitboxes
     * 
     */
    static void Update(void);
};
