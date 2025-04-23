#include "HitboxManager.h"
#include "MapManager.h"

AttackHitbox* HitboxManager::enemyAtHitboxes[MAX_HITBOXES] = {};
ColliderHitbox* HitboxManager::enemyCoHitboxes[MAX_HITBOXES] = {};
u32 HitboxManager::numEnemyAtHitboxes = 0;
u32 HitboxManager::numEnemyCoHitboxes = 0;

void HitboxManager::RegisterAttackHitbox(AttackHitbox* const at) {
    enemyAtHitboxes[numEnemyAtHitboxes++] = at;
}

void HitboxManager::RegisterColliderHitbox(ColliderHitbox* const co) {
    enemyCoHitboxes[numEnemyCoHitboxes++] = co;
}

void HitboxManager::Update(void) {
    // TODO: Implement method of obtaining the player instance
    for(u32 i = 0; i < numEnemyAtHitboxes; i++) {
        /*
        if(enemyAtHitboxes[i]->isActive && enemyAtHitboxes[i]->getGlobalBounds().intersects(PlayerManager::GetPlayer().GetCoHitbox()->getGlobalBounds()))
            PlayerManager::GetPlayer().OnHit();
        */
    }

    /*
    if(PlayerManager::GetPlayer().GetAtHitbox()->isActive) {
        for(u32 i = 0; i < numEnemyCoHitboxes; i++) {
            if(enemyCoHitboxes[i]->getGlobalBounds().intersects(PlayerManager::GetPlayer().GetAtHitbox()->getGlobalBounds())) {
                enemyCoHitboxes[i]->RunHitFunction(PlayerManager::GetPlayer().GetAtHitbox());
            }
        }
    }
    */

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
