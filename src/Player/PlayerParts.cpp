#include <stdexcept>
#include "PlayerParts.h"

PlayerParts::PlayerParts(sf::Vector2f position)
{
    path[0] = "assets/player/hair_upper.png";
    path[1] = "assets/player/plate_full.png";
    path[2] = "assets/player/hand.png";

    length =  sizeof(path) / sizeof(path[0]);

    for (int i = 0; i < length ; i++)
    {
        if (!texture[i].loadFromFile(path[i]))
            throw std::runtime_error("File not found: " + path[i]);

        texture[i].setSmooth(true);
        texture[i].generateMipmap();

        Sprite sp = Sprite(texture[i]);

        sp.setTexture(texture[i]);
        sp.setScale({ 0.05f, 0.05f });
        sp.setPosition(position);

        sprite.push_back(sp);
        if (i == 1)
        {
            sprite[1].setOrigin({ 873.f, 312.f });
            sprite[1].setScale({0.05f, 0.05f});
            sprite[1].setPosition({ position.x+xOffset , position.y });
        }
        if (i == 2)
        {
            sprite[2].setOrigin({ 829.f, 1752.f });
            sprite[2].setScale({0.05f, 0.05f});
            sprite[2].setPosition({ position.x+xOffset , position.y });
        }
    }
}

void PlayerParts::updateParts(sf::Vector2f position)
{
    for (int i = 0; i < length; i++)
    {
        if (i == 1)
        {
            sprite[1].setPosition({ position.x + 42.5f , position.y+13.5f });
        }
        else  if (i == 2)
        {
            sprite[2].setPosition({ position.x + xOffset , position.y +yOffset});
        }
        else
            sprite[i].setPosition(position);
    }
}

void PlayerParts::drawBehindParts(sf::RenderWindow& window)
{
    float time = static_cast<float>(clock()) / 1000.0f;

    float wave = sin(time * rotationSpeed) * rotationAmount;

    for (int i = 0; i < 2; i++)
    {
        if (i == 1)
        {
            sprite[i].setRotation(sf::degrees(wave));

        }
        window.draw(sprite[i]);
    }
}

void PlayerParts::drawFrontParts(sf::RenderWindow& window)
{
    float time = static_cast<float>(clock()) / 1000.0f;

    float wave = sin(time * rotationSpeed) * rotationAmount;

    for (int i = 2; i < length; i++)
    {

        if (i == 2)
        {
            sprite[i].setRotation(sf::degrees(wave));

        }
        window.draw(sprite[i]);
    }
}