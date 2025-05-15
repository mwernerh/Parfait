#include "Framework_Managers/TextureManager.h"
#include <cstdint>
#include <iostream>

namespace {
    /**
     * @brief Hashes the relative path to the desired texture file
     * 
     * @param string 
     * @return constexpr u32 
     */
    constexpr u32 GetHash(const std::string& string) {
        u32 hash = 0;
        u32 directoryLevel = 0;

        // Use prime numbers for weights
        constexpr u32 weights[] = {1, 13, 19, 23, 29, 31, 37};

        for(std::size_t i = string.length(); i > 0; i--) {
            if(string[i - 1] == '/') {
                directoryLevel++;
                continue;
            }

            hash += string[i - 1] * weights[directoryLevel];
        }

        return hash;
    }
}

void TextureManager::TextureManagerWrapper::LRUQueue::Enqueue(const u8 element) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();

    // Place the desired element at the back of the queue

    for(u32 i = 0; i < MAX_TEXTURES; i++) {
        if(tM.leastRecentlyUsedQueue.array[(tM.leastRecentlyUsedQueue.front + i) % MAX_TEXTURES] == element) {
            for(u32 j = i; j < MAX_TEXTURES - 1; j++) {
                tM.leastRecentlyUsedQueue.array[(tM.leastRecentlyUsedQueue.front + i) % MAX_TEXTURES] = tM.leastRecentlyUsedQueue.array[(tM.leastRecentlyUsedQueue.front + i + 1) % MAX_TEXTURES];
            }
            break;
        }
    }

    tM.leastRecentlyUsedQueue.array[(tM.leastRecentlyUsedQueue.front + MAX_TEXTURES - 1) % MAX_TEXTURES] = element;
}

u8 TextureManager::TextureManagerWrapper::LRUQueue::Dequeue(void) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();
    
    // Get element at front of queue
    u8 element = tM.leastRecentlyUsedQueue.array[tM.leastRecentlyUsedQueue.front];
    tM.leastRecentlyUsedQueue.front = (tM.leastRecentlyUsedQueue.front + 1) % MAX_TEXTURES;

    return element;
}

TextureManager::TextureManagerWrapper& TextureManager::GetTextureManagerWrapper(void) {
    static TextureManagerWrapper tm;
    return tm;
}

void TextureManager::Initialize() {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();
    
    tM.leastRecentlyUsedQueue.front = 0;
    for(u32 i = 0; i < MAX_TEXTURES; i++)
        tM.leastRecentlyUsedQueue.array[i] = i;
}

const sf::Texture& TextureManager::LoadTexture(const std::string& texture_dir) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();
    const u32 idx = tM.leastRecentlyUsedQueue.Dequeue();

    [[unlikely]]
    if(!tM.textures[idx].loadFromFile(std::string("./assets/txr/") + texture_dir)) {
        std::cerr << "\x1b[31mError! Could not load texture file " << std::string("./assets/txr/") + texture_dir << " when preparing a Texture!\x1b[0m\n";
        return tM.textures[0];
    }

    tM.textureHashes[idx] = GetHash(texture_dir);
    return tM.textures[idx];
}

const sf::Texture& TextureManager::GetTexture(const std::string& texture_dir) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();
    u32 hash = GetHash(texture_dir);

    for(u32 i = 0; i < MAX_TEXTURES; i++) {
        if(tM.textureHashes[i] == hash) {
            tM.leastRecentlyUsedQueue.Enqueue(i);
            return tM.textures[i];
        }
    }

    return LoadTexture(texture_dir);
}

u32 TextureManager::CalculateTextureHash(const std::string& texture_dir) {
    return GetHash(texture_dir);
}

u32 TextureManager::FindTextureHash(const std::string& texture_dir) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();

    const u32 hash = GetHash(texture_dir);

    for(u32 i = 0; i < MAX_TEXTURES; i++) {
        if(hash == tM.textureHashes[i]) {
            return tM.textureHashes[i];
        }
    }

    std::cerr << "\x1b[31mError! Could not find " << texture_dir << " in loaded textures!\x1b[0m\n";

    // Texture not found
    return static_cast<u32>(-1);
}

const u32& TextureManager::FindTextureHash(const sf::Texture& texture) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();

    uintptr_t offset = std::bit_cast<uintptr_t>(&texture);
    uintptr_t base = std::bit_cast<uintptr_t>(&tM.textures[0]);
    u32 index = (offset - base) / sizeof(sf::Texture);

    return tM.textureHashes[index];
}

const u32& TextureManager::FindTextureHash(const sf::Texture* const texture) {
    TextureManagerWrapper& tM = GetTextureManagerWrapper();

    uintptr_t offset = std::bit_cast<uintptr_t>(texture);
    uintptr_t base = std::bit_cast<uintptr_t>(&tM.textures[0]);
    u32 index = (offset - base) / sizeof(sf::Texture);

    return tM.textureHashes[index];
}