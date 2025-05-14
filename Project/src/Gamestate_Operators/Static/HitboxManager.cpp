#include "Gamestate_Operators/Static/HitboxManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Framework_Managers/AudioManager.h"
#include "Framework_Managers/InputManager.h"
#include "Player.h"
#include "SFML/System/Vector2.hpp"
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
f32 HitboxManager::enemyApproachSFXTimer = HitboxManager::ENEMY_APPROACH_SFX_TIMER_MAX;

void HitboxManager::RegisterAttackHitbox(AttackHitbox* const at) {
    enemyAtHitboxes[numEnemyAtHitboxes++] = at;
}

void HitboxManager::RegisterColliderHitbox(ColliderHitbox* const co) {
    enemyCoHitboxes[numEnemyCoHitboxes++] = co;
}

void HitboxManager::Update(void) {
    enemyApproachSFXTimer -= InputManager::GetDeltaTime();

    f32 closestEnemyDistance = std::numeric_limits<f32>::infinity();
    const AttackHitbox* closestEnemyAttackHitbox = nullptr;

    for(u32 i = 0; i < numEnemyAtHitboxes; i++) {
        if(enemyAtHitboxes[i]->isActive && enemyAtHitboxes[i]->getGlobalBounds().intersects(PlayerManager::GetPlayer().getColliderHitbox().getGlobalBounds()))
            PlayerManager::GetPlayer().takeDamage(const_cast<Player*>(&PlayerManager::GetPlayer()), enemyAtHitboxes[i]);

        sf::Vector2f distVec = PlayerManager::GetPlayer().getPosition() - enemyAtHitboxes[i]->getPosition();
        

        const f32 distToPlayer = std::sqrt(std::pow(distVec.x, 2.0f) + std::pow(distVec.y, 2.0f));

        if(closestEnemyDistance > distToPlayer) {
            closestEnemyDistance = distToPlayer;
            closestEnemyAttackHitbox = enemyAtHitboxes[i];
        }
    }
    
    for(u32 i = 0; i < numEnemyCoHitboxes; i++) {
        // Check if player is attacking and whether their attack hitbox overlaps with the registered enemy collider hitboxes
        if(PlayerManager::GetPlayer().getAttackHitbox().isActive && enemyCoHitboxes[i]->getGlobalBounds().intersects(PlayerManager::GetPlayer().getAttackHitbox().getGlobalBounds())) {
		    enemyCoHitboxes[i]->RunHitFunction(&PlayerManager::GetPlayer().getAttackHitbox());
        }
    }

    // Check if the enemy is a certain distance from the player and alert them
    if(enemyApproachSFXTimer < 0.0f && closestEnemyAttackHitbox && closestEnemyDistance <= 800.0f) {
        f32 pitch = 1.0f;
        if(closestEnemyDistance > 700.0f)
            pitch = 1.2f;
        else if(closestEnemyDistance > 550.0f)
            pitch = 1.4f;
        else
            pitch = 1.6f;

        std::cout << "Approach SFX!\n";

        switch(closestEnemyAttackHitbox->GetAttackerType()) {
            case AttackHitbox::AttackerType::DOG_BLACK:
            case AttackHitbox::AttackerType::DOG_YELLOW:
                AudioManager::StartPositionalSound("a_e_dg", {closestEnemyAttackHitbox->getGlobalBounds().left, closestEnemyAttackHitbox->getGlobalBounds().top, 0.0f}, pitch);
                break;
            case AttackHitbox::AttackerType::BIRD:
                AudioManager::StartPositionalSound("a_e_bd", {closestEnemyAttackHitbox->getGlobalBounds().left, closestEnemyAttackHitbox->getGlobalBounds().top, 0.0f}, pitch);
                break;
            case AttackHitbox::AttackerType::RAT:
                AudioManager::StartPositionalSound("a_e_rt", {closestEnemyAttackHitbox->getGlobalBounds().left, closestEnemyAttackHitbox->getGlobalBounds().top, 0.0f}, pitch);
                break;
            default:
                break;
        }

        enemyApproachSFXTimer = ENEMY_APPROACH_SFX_TIMER_MAX;
    }

    numEnemyAtHitboxes = numEnemyCoHitboxes = 0;
}
