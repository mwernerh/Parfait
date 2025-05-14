#pragma once
#include "Literals.h"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

class TextureManager {
    static constexpr u32 MAX_TEXTURES = 100;

    struct TextureManagerWrapper {
        sf::Texture textures[MAX_TEXTURES];
        u32 textureHashes[MAX_TEXTURES];

        struct LRUQueue {
            u8 array[MAX_TEXTURES];
            u8 front;

            /**
             * @brief Pushes an element, which is expected to be [0, MAX_TEXTURES), to the back of the least-recently used queue of the Textures
             * 
             * @param element 
             */
            void Enqueue(const u8 element);

            /**
             * @brief Gets the index of the Texture in the textures array which has been used the least as of recent
             * 
             * @return u8 
             */
            u8 Dequeue(void);
        } leastRecentlyUsedQueue;
    };

    static TextureManagerWrapper& GetTextureManagerWrapper(void);
    static const sf::Texture& LoadTexture(const std::string& texture_dir);

    template <const u32 NUM_TEXTURES>
    static const std::array<const sf::Texture*, NUM_TEXTURES> LoadTextures(const std::string& directory, const std::array<const std::string, NUM_TEXTURES>& texture_names) {
        TextureManagerWrapper& tM = GetTextureManagerWrapper();
        std::array<const sf::Texture*, NUM_TEXTURES> ret = {};

        u32 currentIdx;
        for(u32 i = 0; i < NUM_TEXTURES; i++) {
            currentIdx = tM.leastRecentlyUsedQueue.Dequeue();
            [[unlikely]]
            if(!tM.textures[currentIdx].loadFromFile(std::string("./assets/txr/") + directory + texture_names[i] + std::string(".png"))) {
                std::cerr << "\x1b[31mError! Could not load texture file " << std::string("./assets/txr/") + directory + texture_names[i] << " when preparing a Texture!\x1b[0m\n";
                ret[i] = nullptr;
            }
            else [[likely]] {
                ret[i] = &tM.textures[currentIdx];
                tM.textureHashes[currentIdx] = CalculateTextureHash(directory) + CalculateTextureHash(texture_names[i]) + CalculateTextureHash(std::string(".png"));
            }
    }

        return std::bit_cast<const std::array<const sf::Texture*, NUM_TEXTURES>>(ret);
    }

    public:
    static void Initialize(void);
    static const sf::Texture& GetTexture(const std::string& texture_dir);
    static u32 CalculateTextureHash(const std::string& texture_dir);
    static u32 FindTextureHash(const std::string& texture_dir);
    static const u32& FindTextureHash(const sf::Texture& texture);
    static const u32& FindTextureHash(const sf::Texture* const texture);

    template <const u32 NUM_TEXTURES>
    static const std::array<const sf::Texture* const, NUM_TEXTURES> GetTextures(const std::string& directory, const std::array<const std::string, NUM_TEXTURES>& texture_names) {
        TextureManagerWrapper& tM = GetTextureManagerWrapper();
        std::array<const sf::Texture*, NUM_TEXTURES> ret = {};
        const u32 directoryHash = CalculateTextureHash(directory);

        u32 hashes[NUM_TEXTURES];
        u32 numUnloaded = NUM_TEXTURES;
        
        for(u32 j = 0; j < NUM_TEXTURES; j++)
            hashes[j] = directoryHash + CalculateTextureHash(texture_names[j]);
        
        for(u32 i = 0; i < MAX_TEXTURES; i++) {
            for(u32 j = 0; j < NUM_TEXTURES; j++) {
                if(tM.textureHashes[i] == hashes[j]) {
                    ret[j] = &tM.textures[i];
                    numUnloaded -= 1;
                }
            }
        }


        if(numUnloaded == NUM_TEXTURES) {
            ret = LoadTextures<NUM_TEXTURES>(directory, texture_names);
        }
        else if(numUnloaded > 0) {
            for(u32 j = 0; j < NUM_TEXTURES; j++) {
                if(!ret[j])
                    ret[j] = &LoadTexture(directory + texture_names[j]);
            }
        }

        return std::bit_cast<std::array<const sf::Texture* const, NUM_TEXTURES>>(ret);
    }
};