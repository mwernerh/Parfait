#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class Enemy{
    public:

    // Constructor
    Enemy(float x, float y, std::shared_ptr<sf::Texture> texture){

        void update();
        void draw();
        void validDistance();

    }

    private:

    std::shared_ptr<sf::Texture> texture; //Texture for the enemy
    sf:: Sprite sprite; //Sprite for the enemy
    float speed;
    int hralth;
    int maxHealth;

    int currentFrame;
    int timePerFrame;
    float timeSinceLastFrame;
    bool alive;
    //Hitbox hitbox;

    

};