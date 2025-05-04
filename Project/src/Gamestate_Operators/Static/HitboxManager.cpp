#include "Gamestate_Operators/Static/HitboxManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/InputManager.h"
#include "Player.h"
#include <cmath>
#include <cstring>
#include <limits>
#include <iostream>

/**
 * @author Mar Werner Hernandez
 */

AttackHitbox* HitboxManager::enemyAtHitboxes[MAX_HITBOXES] = {};
ColliderHitbox* HitboxManager::enemyCoHitboxes[MAX_HITBOXES] = {};
u32 HitboxManager::numEnemyAtHitboxes = 0;
u32 HitboxManager::numEnemyCoHitboxes = 0; 
f32 HitboxManager::enemyApproachSFXTimer = HitboxManager::ENEMY_APPROACH_SFX_TIMER_MAX;

void HitboxManager::RegisterAttackHitbox(AttackHitbox* const at) {
    enemyAtHitboxes[numEnemyAtHitboxes++] = at;
}

void HitboxManager::RegisterColliderHitbox(ColliderHitbox* const co) {
    enemyCoHitboxes[numEnemyCoHitboxes++] = co;
}

void HitboxManager::Update(void) {
    enemyApproachSFXTimer -= InputManager::GetDeltaTime();

    for(u32 i = 0; i < numEnemyAtHitboxes; i++) {
        /*
        if(enemyAtHitboxes[i]->isActive && enemyAtHitboxes[i]->getGlobalBounds().intersects(PlayerManager::GetPlayer().GetCoHitbox()->getGlobalBounds()))
            PlayerManager::GetPlayer().OnHit();
        */
    }


    f32 closestEnemyDistance = std::numeric_limits<f32>::infinity();
    const ColliderHitbox* closestEnemyCollider = nullptr;
    
    for(u32 i = 0; i < numEnemyCoHitboxes; i++) {
        // Check if player is attacking and whether their attack hitbox overlaps with the registered enemy collider hitboxes
        if(PlayerManager::GetPlayer().getAttackHitbox().isActive && enemyCoHitboxes[i]->getGlobalBounds().intersects(PlayerManager::GetPlayer().getAttackHitbox().getGlobalBounds())) {
		    enemyCoHitboxes[i]->RunHitFunction(&PlayerManager::GetPlayer().getAttackHitbox());
        }

        const f32 distToPlayer = std::abs(PlayerManager::GetPlayer().getPosition().x - enemyCoHitboxes[i]->getPosition().x);

        if(closestEnemyDistance > distToPlayer) {
            closestEnemyDistance = distToPlayer;
            closestEnemyCollider = enemyCoHitboxes[i];
        }
    }

    // Check if the enemy is a certain distance from the player and alert them
    if(enemyApproachSFXTimer < 0.0f && closestEnemyCollider && closestEnemyDistance <= 800.0f) {
        f32 pitch = 1.0f;
        if(closestEnemyDistance > 700.0f)
            pitch = 1.2f;
        else if(closestEnemyDistance > 550.0f)
            pitch = 1.4f;
        else
            pitch = 1.6f;

        AudioManager::StartPositionalSound("e_approach", {closestEnemyCollider->getGlobalBounds().left, closestEnemyCollider->getGlobalBounds().top, 0.0f}, pitch);
        enemyApproachSFXTimer = ENEMY_APPROACH_SFX_TIMER_MAX;
    }

    numEnemyAtHitboxes = numEnemyCoHitboxes = 0;
}
