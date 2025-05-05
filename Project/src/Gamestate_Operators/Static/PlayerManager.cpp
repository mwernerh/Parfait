#include "Gamestate_Operators/Static/PlayerManager.h"
#include "Framework_Managers/InputManager.h"

Player& PlayerManager::GetPlayerInternal(void) {
    static Player player("./assets/txr/animals/ct2/Walk.png", 20);
    return player;
}

const Player& PlayerManager::GetPlayer(void) {
    return GetPlayerInternal();
}

void PlayerManager::Initialize(void) {
    GetPlayerInternal();
}

void PlayerManager::Update(void) {
    GetPlayerInternal().update(InputManager::GetDeltaTime());
}

void PlayerManager::Draw(sf::RenderWindow& window) {
    GetPlayerInternal().draw(window);
}
