#include "Gamestate_Operators/Static/MapManager.h"
#include "Framework_Managers/AudioManager.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <iostream>
#include <SFML/OpenGL.hpp>

/**
 * @author Mar Werner Hernandez
 */

void MapManager::Initialize(void) {
    ChangeMap(Maps::CITY);
}

void MapManager::Draw(sf::RenderWindow& window) {
    // TODO: Obtain player instance to remove paramaters other than the RenderWindow
    ParallaxBackground& bg = GetBackground();

    const sf::Vector2f& viewSize = window.getView().getSize();
    const sf::Vector2f& cameraPos = window.getView().getCenter();

    constexpr f32 backgroundSlowdownFraction = 1.0f/2.5f;

    // Place sprite at player position, then draw it
    for(u32 i = 0; i < ParallaxBackground::NUM_PARALLAX_LAYERS; i++) {
        bg.s_layers[i].setPosition(cameraPos);


        // Texture coordinates are integers, but player movement is fractional -- store fractional offset just in case player movement yields a change in offset between 0 & 1
        sf::IntRect texRect = GetBackground().s_layers[i].getTextureRect();

        const f32 backgroundFraction = (static_cast<float>(i + 1)/static_cast<float>(ParallaxBackground::NUM_PARALLAX_LAYERS));

        f32 textureRectangleHorizontalOffset = (cameraPos.x * backgroundFraction * backgroundSlowdownFraction) - (viewSize.x / 2.0f);

        if(textureRectangleHorizontalOffset * (i / 4.0f) >= static_cast<float>(texRect.width))
            textureRectangleHorizontalOffset -= static_cast<float>(texRect.width) * std::floor((textureRectangleHorizontalOffset * backgroundFraction) / texRect.width);
        else if(textureRectangleHorizontalOffset * (i / 4.0f) < 0.0f)
            textureRectangleHorizontalOffset += static_cast<float>(texRect.width) * std::floor(std::abs(textureRectangleHorizontalOffset * backgroundFraction) / texRect.width);

        // Truncate intermediate offset to integer for texture coordinates
        texRect.left = static_cast<int>(textureRectangleHorizontalOffset);
        bg.s_layers[i].setTextureRect(texRect);
        window.draw(GetBackground().s_layers[i]);
    }
}

void MapManager::Update(void) {
    // Update environment collision
}

void MapManager::ChangeMap(const Maps mapIdx) {
    ParallaxBackground& bg = GetBackground();

    // Get directory to assets for desired map
    std::string texBaseStr = "./assets/txr/bgs/";
    switch(mapIdx) {
        case Maps::CITY:
            texBaseStr.append("p_ct/1/Night/");
            break;
        case Maps::PARK:
            texBaseStr.append("p_pk/");
            break;
        case Maps::PLAINS:
            texBaseStr.append("p_pl/");
            break;
        default:
            break;
    }

    // Load textures for current map, and make them wrap horizontally
    glEnable(GL_TEXTURE_2D);
    for(u32 i = 0; i < ParallaxBackground::NUM_PARALLAX_LAYERS; i++) {
        if(!bg.t_layers[i].loadFromFile(texBaseStr + std::to_string(i + 1) + ".png")) {
            std::cerr << "\x1b[31mError! Could not load background layer " + std::to_string(i) + "'s texture!\x1b[0m\n";
            continue;
        }

        // Only wrap horizontally (s texture coordinate)
        sf::Texture::bind(&bg.t_layers[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    }

    // Unbind GL context from any textures
    sf::Texture::bind(nullptr);
    
    // Assign textures to sprites, center origin, and place them at origin
    for(u32 i = 0; i < ParallaxBackground::NUM_PARALLAX_LAYERS; i++) {
        sf::Vector2u currentTexSize = GetBackground().t_layers[i].getSize();
        bg.s_layers[i].setTexture(GetBackground().t_layers[i], true);
        bg.s_layers[i].setOrigin({static_cast<float>(currentTexSize.x) / 2, static_cast<float>(currentTexSize.y) / 2});
        bg.s_layers[i].setPosition({1280.0f / 2.0f, 720.0f / 2.0f});
        bg.s_layers[i].setScale(1280.0f / currentTexSize.x, 720.0f / currentTexSize.y);
    }

    AudioManager::ClearMusic();
    switch(mapIdx) {
        case Maps::CITY:
            AudioManager::StartMusic("amb_ct");
            AudioManager::StartMusic("bgm_lofi");
            break;
        case Maps::PARK:
            AudioManager::StartMusic("amb_pk");
            AudioManager::StartMusic("bgm_accordion");
            break;
        case Maps::PLAINS:
            AudioManager::StartMusic("amb_pl");
            AudioManager::StartMusic("bgm_piano");
            break;
        default:
            break;
    }
}

MapManager::ParallaxBackground& MapManager::GetBackground(void) {
    static MapManager::ParallaxBackground bg;
    return bg;
}