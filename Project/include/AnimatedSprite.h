#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <array>
#include "Literals.h"
#include "Framework_Managers/TextureManager.h"
#include "Framework_Managers/AnimationManager.h"

template <const std::size_t LENGTH>
struct __StrLit {
    consteval __StrLit(const char (&string)[LENGTH]) {
        std::copy_n(string, LENGTH, arr);
    }

    std::size_t length(void) { return LENGTH; }

    template<const std::size_t LENGTH_RHS>
    constexpr bool operator==(const __StrLit<LENGTH_RHS>& rhs) const {
        if(LENGTH_RHS != LENGTH)
            return false;

        for(u32 i = 0; i < LENGTH; i++) {
            if(rhs.arr[i] != arr[i])
                return false;
        }

        return true;
    }

    char arr[LENGTH];
};

template <const std::size_t LENGTH>
struct AnimationDescription {
    __StrLit<LENGTH> name;
    u32 numFrames;
};

template <const __StrLit BASE_DIRECTORY, AnimationDescription... ANIMATIONS>
requires (sizeof...(ANIMATIONS) > 0)
class AnimatedSprite : public sf::Sprite {
    void SetDefaultAnimation(const sf::Texture* const texture, const u32 numFrames) {
        setTexture(*texture, true);

        const s32 FRAME_WIDTH = getTextureRect().width / numFrames;

        setTextureRect({0, 0, FRAME_WIDTH, getTextureRect().height});
    }

    public:
        AnimatedSprite(void) {
            const std::string baseDir = BASE_DIRECTORY.arr;

            const std::array<const std::string, sizeof...(ANIMATIONS)> textureNames = {(ANIMATIONS.name.arr)...};
            constexpr u32 ANIMATION_LENGTHS[sizeof...(ANIMATIONS)] = {(ANIMATIONS.numFrames)...}; 

            // Ensure the textures are loaded
            auto defaultTexture = TextureManager::GetTextures<sizeof...(ANIMATIONS)>(baseDir, textureNames)[0];

            if(defaultTexture)
                SetDefaultAnimation(defaultTexture, ANIMATION_LENGTHS[0]);
        }

        u32 getTextureHash(void) {
            return TextureManager::FindTextureHash(getTexture());
        }

        template <const __StrLit ANIMATION_NAME>
        requires(((ANIMATION_NAME == ANIMATIONS.name) || ...))
        void SetAnimation(u32 startingFrame = 0) {
            // Don't do anything if the desired animation is already the animation that is playing for the sprite
            std::string file = BASE_DIRECTORY.arr;
            file.append(ANIMATION_NAME.arr);
            file.append(".png");

            if(getTextureHash() == TextureManager::FindTextureHash(file)) {
                return;
            }

            setTexture(TextureManager::GetTexture(file), true);

            u32 numFrames = 0;
            ((numFrames = (ANIMATION_NAME == ANIMATIONS.name ? ANIMATIONS.numFrames : numFrames)), ...);

            const s32 FRAME_WIDTH = getTextureRect().width / numFrames;

            setTextureRect({static_cast<int>(FRAME_WIDTH * startingFrame), 0, FRAME_WIDTH, getTextureRect().height});
        }

        void Update() {
            if(AnimationManager::ShouldUpdateAnimation())
                setTextureRect({static_cast<int>(getTextureRect().left + getTextureRect().width) % static_cast<int>(getTexture()->getSize().x), 0, getTextureRect().width, getTextureRect().height});
        }
};

#define ANIM_DESC(str, frm) {__StrLit(#str),frm}

using AnimatedBirdSprite1 = AnimatedSprite<"./assets/txr/animals/bd1/", ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;
using AnimatedBirdSprite2 = AnimatedSprite<"./assets/txr/animals/bd2/", ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;

using AnimatedCatSprite1 = AnimatedSprite<"./assets/txr/animals/ct1/", ANIM_DESC(Attack, 4), ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;
using AnimatedCatSprite2 = AnimatedSprite<"./assets/txr/animals/ct2/", ANIM_DESC(Attack, 4), ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;

using AnimatedDogSprite1 = AnimatedSprite<"./assets/txr/animals/dg1/", ANIM_DESC(Attack, 4), ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;
using AnimatedDogSprite2 = AnimatedSprite<"./assets/txr/animals/dg2/", ANIM_DESC(Attack, 4), ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;

using AnimatedRatSprite1 = AnimatedSprite<"./assets/txr/animals/rt1/", ANIM_DESC(Death, 4), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;
using AnimatedRatSprite2 = AnimatedSprite<"./assets/txr/animals/rt2/", ANIM_DESC(Death, 2), ANIM_DESC(Hurt, 2), ANIM_DESC(Idle, 4), ANIM_DESC(Walk, 6)>;