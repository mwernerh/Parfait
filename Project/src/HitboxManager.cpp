#include "HitboxManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include <cmath>
#include <cstring>
#include <limits>

/**
 * @author Mar Werner Hernandez
 */

AttackHitbox* HitboxManager::enemyAtHitboxes[MAX_HITBOXES] = {};
ColliderHitbox* HitboxManager::enemyCoHitboxes[MAX_HITBOXES] = {};
u32 HitboxManager::numEnemyAtHitboxes = 0;
u32 HitboxManager::numEnemyCoHitboxes = 0; 
Player* HitboxManager::player = nullptr;
f32 HitboxManager::enemyApproachSFXTimer = HitboxManager::ENEMY_APPROACH_SFX_TIMER_MAX;

void HitboxManager::RegisterPlayer(Player* player)
{
	HitboxManager::player = player;
}

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
        if(player->getAttackHitbox().isActive && enemyCoHitboxes[i]->getGlobalBounds().intersects(player->getAttackHitbox().getGlobalBounds())) {
		    enemyCoHitboxes[i]->RunHitFunction(&player->getAttackHitbox());
        }

        const f32 distToPlayer = std::abs(player->getPosition().x - enemyCoHitboxes[i]->getPosition().x);

        if(closestEnemyDistance > distToPlayer) {
            closestEnemyDistance = distToPlayer;
            closestEnemyCollider = enemyCoHitboxes[i];
        }
    }

    // Check if the enemy is a certain distance from the player and alert them
    if(enemyApproachSFXTimer < 0.0f && closestEnemyCollider && closestEnemyDistance <= 800.0f) {
        std::cout << "Alert enemy approach!\n";
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

    /*
    if(MapManager::GetCollision().getBounds().intersects(PlayerManager::GetPlayer().GetCoHitbox()->getGlobalBounds())) {
        // Set player to be above ground
    }
    */

    /*
    for(u32 i = 0; i < numEnemyCoHitboxes; i++) {
        if(enemyCoHitboxes[i]->getGlobalBounds().intersects(MapManager::GetCollision().getBounds()) {
            // Set enemy to be above ground
        }
    }
    */

    numEnemyAtHitboxes = numEnemyCoHitboxes = 0;
}
