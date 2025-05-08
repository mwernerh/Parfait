#include "Framework_Managers/FontManager.h"
#include <iostream>

namespace {
    /**
     * @brief Hashes the relative path to the desired font file
     * 
     * @param string 
     * @return constexpr u32 
     */
    constexpr u32 GetHash(const std::string& string) {
        u32 hash = 0;
        u32 directoryLevel = 0;

        // Use prime numbers for weights
        constexpr u32 weights[] = {1, 17, 19, 23, 29, 31, 37};

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

void FontManager::FontManagerWrapper::LRUQueue::Enqueue(const u8 element) {
    FontManagerWrapper& fM = GetFontManagerWrapper();

    // Place the desired element at the back of the queue

    for(u32 i = 0; i < MAX_FONTS; i++) {
        if(fM.leastRecentlyUsedQueue.array[(fM.leastRecentlyUsedQueue.front + i) % MAX_FONTS] == element) {
            for(u32 j = i; j < MAX_FONTS - 1; j++) {
                fM.leastRecentlyUsedQueue.array[(fM.leastRecentlyUsedQueue.front + i) % MAX_FONTS] = fM.leastRecentlyUsedQueue.array[(fM.leastRecentlyUsedQueue.front + i + 1) % MAX_FONTS];
            }
            break;
        }
    }

    fM.leastRecentlyUsedQueue.array[(fM.leastRecentlyUsedQueue.front + MAX_FONTS - 1) % MAX_FONTS] = element;
}

u8 FontManager::FontManagerWrapper::LRUQueue::Dequeue(void) {
    FontManagerWrapper& fM = GetFontManagerWrapper();
    
    // Get element at front of queue
    u8 element = fM.leastRecentlyUsedQueue.array[fM.leastRecentlyUsedQueue.front];
    fM.leastRecentlyUsedQueue.front = (fM.leastRecentlyUsedQueue.front + 1) % MAX_FONTS;

    return element;
}

FontManager::FontManagerWrapper& FontManager::GetFontManagerWrapper(void) {
    static FontManagerWrapper fM;
    return fM;
}

void FontManager::Initialize(void) {
    FontManagerWrapper& fM = GetFontManagerWrapper();

    for(u32 i = 0; i < MAX_FONTS; i++) {
        fM.leastRecentlyUsedQueue.array[i] = i;
    }
}

const sf::Font& FontManager::LoadFont(const std::string& font_name) {
    FontManagerWrapper& fM = GetFontManagerWrapper();
    const u32 idx = fM.leastRecentlyUsedQueue.Dequeue();

    if(!fM.fonts[idx].loadFromFile(FONT_DIRECTORY + font_name + std::string(".otf"))) {
        std::cerr << "\x1b[31mError! Could not load font file " << font_name << " when preparing a Font!\x1b[0m\n";
        return fM.fonts[0];
    }

    fM.fontHashes[idx] = GetHash(font_name);

    return fM.fonts[idx];
}

const sf::Font& FontManager::GetFont(const std::string& font_name) {
    FontManagerWrapper& fM = GetFontManagerWrapper();
    
    const u32 desiredHash = GetHash(font_name);

    for(u32 i = 0; i < MAX_FONTS; i++) {
        if(fM.fontHashes[i] == desiredHash) {
            fM.leastRecentlyUsedQueue.Enqueue(i);
            return fM.fonts[i];
        }
    }

    // Font not loaded -- must load it
    return LoadFont(font_name);
}