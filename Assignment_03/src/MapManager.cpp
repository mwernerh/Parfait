#include "MapManager.h"
#include "EnvironmentCollision.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <iostream>
#include <SFML/OpenGL.hpp>

/**
 * @author Mar Werner Hernandez
 */

void MapManager::Draw(sf::RenderWindow& window, sf::Vector2f playerPos, float playerHorizontalVelocity) {
    // TODO: Obtain player instance to remove paramaters other than the RenderWindow

    // Place sprite at player position, then draw it
    for(u32 i = 0; i < ParallaxBackground::NUM_PARALLAX_LAYERS; i++) {
        GetBackground().s_layers[i].setPosition(playerPos);
        if(i > 0) {
            sf::IntRect texRect = GetBackground().s_layers[i].getTextureRect();
            texRect.left = (texRect.left + static_cast<int>(playerHorizontalVelocity * (i/4.0f)) % texRect.width);
            GetBackground().s_layers[i].setTextureRect(texRect);
        }
        window.draw(GetBackground().s_layers[i]);
    }
}

void MapManager::Update(void) {
    // Update environment collision
}

void MapManager::ChangeMap(const Maps mapIdx) {
    // Get directory to assets for desired map
    std::string texBaseStr = "./assets/txr/bgs/";
    switch(mapIdx) {
        case Maps::CITY:
            texBaseStr.append("p_ct/1/Night/");
            break;
        case Maps::PARK:
            texBaseStr.append("p_pk/");
            break;
        case Maps::CITY_VAR:
            texBaseStr.append("p_ct/4/Day/");
            break;
        default:
            break;
    }

    // Load textures for current map, and make them wrap horizontally
    glEnable(GL_TEXTURE_2D);
    for(u32 i = 0; i < ParallaxBackground::NUM_PARALLAX_LAYERS; i++) {
        if(!GetBackground().t_layers[i].loadFromFile(texBaseStr + std::to_string(i + 1) + ".png")) {
            std::cerr << "\x1b[31mError! Could not load background layer " + std::to_string(i) + "'s texture!\x1b[0m\n";
            continue;
        }

        // Only wrap horizontally
        sf::Texture::bind(&GetBackground().t_layers[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    }
    sf::Texture::bind(nullptr);
    
    // Assign textures to sprites, center origin, and place them at origin
    for(u32 i = 0; i < ParallaxBackground::NUM_PARALLAX_LAYERS; i++) {
        sf::Vector2u currentTexSize = GetBackground().t_layers[i].getSize();
        GetBackground().s_layers[i].setTexture(GetBackground().t_layers[i], true);
        GetBackground().s_layers[i].setOrigin({static_cast<float>(currentTexSize.x) / 2, static_cast<float>(currentTexSize.y) / 2});
        GetBackground().s_layers[i].setPosition({1280.0f / 2.0f, 720.0f / 2.0f});
        GetBackground().s_layers[i].setScale(1280.0f / currentTexSize.x, 720.0f / currentTexSize.y);
    }
}

MapManager::ParallaxBackground& MapManager::GetBackground(void) {
    static MapManager::ParallaxBackground bg;
    return bg;
}

EnvironmentCollision& MapManager::GetCollision(void) {
    static EnvironmentCollision collision;
    return collision;
}