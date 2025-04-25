#include "InputManager.h"
#include "Literals.h"
#include <cstring>

std::bitset<sf::Keyboard::Scancode::ScancodeCount> InputManager::states[2] = {};
bool InputManager::prevStateIdx = 0;
float InputManager::deltaTimeSeconds = 0.0f;


sf::Clock& InputManager::GetDeltaTimeClock(void) {
    static sf::Clock deltaTimeClock;
    return deltaTimeClock;
}

void InputManager::Initialize(void) {
    GetDeltaTimeClock();
}

void InputManager::Update(void) {
    ResetDeltaTimeClock();
    IncrementState();
}

void InputManager::IncrementState(void) {
    // Switch interpretation of arrays (prevState array -> currentState array & vice-versa)
    prevStateIdx = !prevStateIdx;
    states[!prevStateIdx] = states[prevStateIdx];
}

void InputManager::RegisterKeyPress(sf::Keyboard::Scancode key) {
    states[!prevStateIdx][static_cast<u32>(key)] = true;
}

void InputManager::RegisterKeyRelease(sf::Keyboard::Scancode key) {
    states[!prevStateIdx][static_cast<u32>(key)] = false;
}

// Returns when the key is first pressed, but not held
bool InputManager::IsKeyPressed(sf::Keyboard::Scancode key) {
    return states[!prevStateIdx][static_cast<u32>(key)] && !states[prevStateIdx][static_cast<u32>(key)];
}

bool InputManager::IsKeyHeld(sf::Keyboard::Scancode key) {
    return states[!prevStateIdx][static_cast<u32>(key)] && states[prevStateIdx][static_cast<u32>(key)];
}

bool InputManager::IsKeyReleased(sf::Keyboard::Scancode key) {
    return !states[!prevStateIdx][static_cast<u32>(key)] && states[prevStateIdx][static_cast<u32>(key)];
}

void InputManager::ResetDeltaTimeClock(void) {
    deltaTimeSeconds = GetDeltaTimeClock().restart().asSeconds();
}

float InputManager::GetDeltaTime(void) {
    return deltaTimeSeconds;
}