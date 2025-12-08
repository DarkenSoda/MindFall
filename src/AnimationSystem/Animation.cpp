#include "Animation.h"
#include <stdexcept>

Animation::Animation(std::string path, float switchTime, int frameCount)
    : sprite(texture)
{
    if (!texture.loadFromFile(path))
        throw std::runtime_error("File not found: " + path);

    this->switchTime = switchTime;
    this->frameCount = frameCount;

    size = texture.getSize();
    sizeX = size.x / frameCount;
    sizeY = size.y - 3;

    sprite.setScale(sf::Vector2f(3.0f, 3.0f));
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect({ 15, 15 }, { sizeX - 30, sizeY - 30 }));

}

void Animation::Play(float deltaTime, sf::Vector2f position)
{
    sprite.setPosition(position);
    timer += deltaTime;
    if (timer >= switchTime)
    {
        frame++;
        if (frame >= frameCount)
            frame = 0;

        sprite.setTextureRect(sf::IntRect({ (frame * sizeX) + 15, 15 }, { sizeX - 30, sizeY - 30 }));
        timer = 0.0f;
    }
}

sf::Sprite& Animation::getSprite()
{
    return sprite;
}
