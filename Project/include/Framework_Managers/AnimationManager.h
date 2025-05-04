#pragma once
#include "Literals.h"
#include "SFML/System/Clock.hpp"

class AnimationManager {
    static inline constexpr f32 FRAME_RATE = 1.0f / 12.0f;

    struct AnimationManagerWrapper {
        sf::Clock clock;
        bool shouldUpdateAnimation;
    };

    static AnimationManagerWrapper& GetAnimationManagerWrapper(void) {
        static AnimationManagerWrapper aM;
        return aM;
    }

     public:
     static void Initialize(void) {
        AnimationManagerWrapper& aM = GetAnimationManagerWrapper();
        aM.shouldUpdateAnimation = false;
        aM.clock.restart();

     }

     inline static bool ShouldUpdateAnimation(void) { 
        AnimationManagerWrapper& aM = GetAnimationManagerWrapper();
        return aM.shouldUpdateAnimation;
     }

     static void Update(void) {
        AnimationManagerWrapper& aM = GetAnimationManagerWrapper();

        aM.shouldUpdateAnimation = false;
        if(aM.clock.getElapsedTime().asSeconds() > FRAME_RATE) {
            aM.clock.restart();
            aM.shouldUpdateAnimation = true;
        }
     }
};