#pragma once
#include "Literals.h"
#include "SFML/Graphics/Font.hpp"

/**
 * @author Mar Werner Hernandez
 * 
 */

/**
 * @brief Interface to load fonts into memory, minimizing reads from disk
 * 
 */
class FontManager {
    static inline constexpr u32 MAX_FONTS = 10;
    static inline constexpr std::string FONT_DIRECTORY = "./assets/fnt/";

    struct FontManagerWrapper {
        sf::Font fonts[MAX_FONTS];
        u32 fontHashes[MAX_FONTS];

        struct LRUQueue {
            u8 array[MAX_FONTS];
            u8 front;
    
            /**
             * @brief Pushes an element, which is expected to be [0, MAX_FONTS), to the back of the least-recently used queue of the Fonts
             * 
             * @param element 
             */
            void Enqueue(const u8 element);
    
            /**
             * @brief Gets the index of the Fonts in the fonts array which has been used the least as of recent
             * 
             * @return u8 
             */
            u8 Dequeue(void);
        } leastRecentlyUsedQueue;
    };

    /**
     * @brief Get the Font Manager Wrapper singleton
     * 
     * @return FontManagerWrapper& 
     */
    static FontManagerWrapper& GetFontManagerWrapper(void);

    /**
     * @brief Read a font file from disk and load its data into the least recently used font index
     * 
     * @param font_name 
     * @return const sf::Font& 
     */
    static const sf::Font& LoadFont(const std::string& font_name);

    public:
    /**
     * @brief Initialize conditions for LRU queue
     * 
     */
    static void Initialize(void);

    /**
     * @brief Gets the desired font, and loads it if it has not yet beed loaded into memory
     * 
     * @param font_name 
     * @return const sf::Font& 
     */
    static const sf::Font& GetFont(const std::string& font_name);
};