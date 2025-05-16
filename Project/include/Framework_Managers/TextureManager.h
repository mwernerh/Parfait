#pragma once
#include "Literals.h"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Interface to load textures into memory, minimizing reads from disk
 * 
 */
class TextureManager {
    static constexpr u32 MAX_TEXTURES = 50;

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

    /**
     * @brief Get the Texture Manager Wrapper singleton
     * 
     * @return TextureManagerWrapper& 
     */
    static TextureManagerWrapper& GetTextureManagerWrapper(void);

    /**
     * @brief Reads a .png file from the texture directory and stores its data in the texture array
     * 
     * @param texture_dir The directory to the texture, starting implicitly from the txr/ folder of assets. DO NOT INCLUDE .png file extension!
     * @return const sf::Texture& 
     */
    static const sf::Texture& LoadTexture(const std::string& texture_dir);

    /**
     * @brief Reads multiple .png files present in the same directory and stores its data in the texture array
     * 
     * @tparam NUM_TEXTURES 
     * @param directory The directory in which the textures reside, exlcuding the implicit txr/ directory 
     * @param texture_names The names of the desired textures, without file extensions
     * @return const std::array<const sf::Texture*, NUM_TEXTURES> 
     */
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
                // Texture successfully loaded into the textures array -- prepare a pointer to this texture for the return
                ret[i] = &tM.textures[currentIdx];
                tM.textureHashes[currentIdx] = CalculateTextureHash(directory) + CalculateTextureHash(texture_names[i]) + CalculateTextureHash(std::string(".png"));
            }
    }

        return std::bit_cast<const std::array<const sf::Texture*, NUM_TEXTURES>>(ret);
    }

    public:
    /**
     * @brief Setup the LRU queue to the expected conditions
     * 
     */
    static void Initialize(void);

    /**
     * @brief Obtains the desired texture has already been loaded into memory -- otherwise, it first laods it into memory
     * 
     * @param texture_dir 
     * @return const sf::Texture& 
     */
    static const sf::Texture& GetTexture(const std::string& texture_dir);

    /**
     * @brief Hashes a texture, taking into account its directory via a weighted hash
     * 
     * @param texture_dir 
     * @return u32 
     */
    static u32 CalculateTextureHash(const std::string& texture_dir);

    /**
     * @brief Calculates the desired texture's hash from its name. Returns 0xFFFFFFFF if the texture isn't already loaded in memory
     * 
     * @param texture_dir 
     * @return u32 
     */
    static u32 FindTextureHash(const std::string& texture_dir);

    /**
     * @brief Assumes the texture to be present in the texture array, and returns its stored hash
     * 
     * @param texture 
     * @return const u32& 
     */
    static const u32& FindTextureHash(const sf::Texture& texture);

    /**
     * @brief Assumes the texture to be present in the texture array, and returns its stored hash
     * 
     * @param texture 
     * @return const u32& 
     */
    static const u32& FindTextureHash(const sf::Texture* const texture);

    /**
     * @brief Obtains the desired textures if thet have already been loaded into memory -- otherwise, it first laods those missing into memory
     * 
     * @tparam NUM_TEXTURES 
     * @param directory 
     * @param texture_names 
     * @return const std::array<const sf::Texture* const, NUM_TEXTURES> 
     */
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
            // None of the textures have been loaded -- we can batch their loading together
            ret = LoadTextures<NUM_TEXTURES>(directory, texture_names);
        }
        else if(numUnloaded > 0) {
            // Some textures are loaded and others aren't -- need to do on a case-by-case basis
            for(u32 j = 0; j < NUM_TEXTURES; j++) {
                if(!ret[j])
                    ret[j] = &LoadTexture(directory + texture_names[j]);
            }
        }

        return std::bit_cast<std::array<const sf::Texture* const, NUM_TEXTURES>>(ret);
    }
};