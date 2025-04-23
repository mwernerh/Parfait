#pragma once
#include "Literals.h"
#include "Hitboxes.h"

class HitboxManager {
    static constexpr u32 MAX_HITBOXES = 256;

    static AttackHitbox* enemyAtHitboxes[MAX_HITBOXES];
    static ColliderHitbox* enemyCoHitboxes[MAX_HITBOXES];

    static u32 numEnemyAtHitboxes;
    static u32 numEnemyCoHitboxes;

    public:

    static void RegisterAttackHitbox(AttackHitbox* const at);
    static void RegisterColliderHitbox(ColliderHitbox* const co);

    static void Update(void);
};