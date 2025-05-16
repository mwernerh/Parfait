#pragma once
#include "Literals.h"
#include "SFML/System/Clock.hpp"

/**
 * @author Mar Werner Hernandez
 * 
 */


/**
 * @brief A global system that can be used to check whether an animation should be updated in the current frame, so that the animation plays at the desired framerate
 * 
 */
class AnimationManager {
    static inline constexpr f32 FRAME_RATE = 1.0f / 12.0f;

    struct AnimationManagerWrapper {
        sf::Clock clock;
        bool shouldUpdateAnimation;
    };

    /**
     * @brief Get the Animation Manager Wrapper singleton
     * 
     * @return AnimationManagerWrapper& 
     */
    static AnimationManagerWrapper& GetAnimationManagerWrapper(void) {
        static AnimationManagerWrapper aM;
        return aM;
    }

     public:
     /**
      * @brief Sets inital internal conditions
      * 
      */
     static void Initialize(void) {
        AnimationManagerWrapper& aM = GetAnimationManagerWrapper();
        aM.shouldUpdateAnimation = false;
        aM.clock.restart();

     }

     /**
      * @brief Returns whether animations should be updated in the current frame, based on the framerate specified in AnimationManager
      * 
      * @return true 
      * @return false 
      */
     inline static bool ShouldUpdateAnimation(void) { 
        AnimationManagerWrapper& aM = GetAnimationManagerWrapper();
        return aM.shouldUpdateAnimation;
     }

     /**
      * @brief Checks whether the time needed for the desired frame rate has passed for the current frame
      * 
      */
     static void Update(void) {
        AnimationManagerWrapper& aM = GetAnimationManagerWrapper();

        aM.shouldUpdateAnimation = false;
        if(aM.clock.getElapsedTime().asSeconds() > FRAME_RATE) {
            aM.clock.restart();
            aM.shouldUpdateAnimation = true;
        }
     }
};