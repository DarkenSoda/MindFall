#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Animation {
private:
    sf::Texture texture;
    sf::Sprite sprite;

    int frame = 0;
    float timer = 0.0f;
    float switchTime;
    int frameCount;

    sf::Vector2u size;
    unsigned int sizeX;
    unsigned int sizeY;

public:
    Animation(std::string path, float switchTime, int frameCount);

    void Play(float deltaTime, sf::Vector2f position);

    sf::Sprite& getSprite();
};

