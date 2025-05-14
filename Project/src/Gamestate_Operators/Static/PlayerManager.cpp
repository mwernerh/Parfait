#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Framework_Managers/InputManager.h"

Player& PlayerManager::GetPlayerInternal(void) {
    static Player player;
    return player;
}

const Player& PlayerManager::GetPlayer(void) {
    return GetPlayerInternal();
}

void PlayerManager::Initialize(void) {
    GetPlayerInternal().setShowText(true);
}

void PlayerManager::Update(void) {
    GetPlayerInternal().update(InputManager::GetDeltaTime());
}

void PlayerManager::Draw(sf::RenderWindow& window) {
    GetPlayerInternal().draw(window);
}
