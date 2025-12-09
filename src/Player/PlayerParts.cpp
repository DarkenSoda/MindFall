#include <stdexcept>
#include "PlayerParts.h"

PlayerParts::PlayerParts(sf::Vector2f position)
{
    path[0] = "assets/player/hair_upper.png";
    path[1] = "assets/player/plate_full.png";
    path[2] = "assets/player/hand.png";
    path[3] = "assets/player/wheel.png";
    path[4] = "assets/player/wheel.png";

    length =  sizeof(path) / sizeof(path[0]);

    for (int i = 0; i < length ; i++)
    {
        if (!texture[i].loadFromFile(path[i]))
            throw std::runtime_error("File not found: " + path[i]);

        texture[i].setSmooth(true);
        texture[i].generateMipmap();

        Sprite sp = Sprite(texture[i]);

        if (i == 0)
        {
            sp.setTexture(texture[i]);
            sp.setScale({ 0.05f, 0.05f });
            sp.setPosition(position);
            sprite.push_back(sp);
        }
        if (i == 1)
        {
            sp.setTexture(texture[i]);
            sprite.push_back(sp);
            sprite[1].setOrigin({ 873.f, 312.f });
            sprite[1].setScale({0.05f, 0.05f});
            sprite[1].setPosition({ position.x+xOffset , position.y });
        }
        else if (i == 2)
        {
            sp.setTexture(texture[i]);
            sprite.push_back(sp);
            sprite[2].setOrigin({ 829.f, 1752.f });
            sprite[2].setScale({0.05f, 0.05f});
            sprite[2].setPosition({ position.x+xOffset , position.y });
        }
        else if (i == 3)
        {   
            sp.setTexture(texture[i]);
            sprite.push_back(sp);
            sprite[3].setOrigin({ 130.0f , 113.0f});
            sprite[3].setScale({ 0.07f, 0.07f });
            sprite[3].setPosition({ position.x + 35.0f , position.y + 105.0f });

            sprite.push_back(sp);
            sprite[4].setOrigin({ 130.0f , 113.0f });
            sprite[4].setScale({ 0.07f, 0.07f });
            sprite[4].setPosition({ position.x + 35.0f , position.y + 105.0f });
        }
        else if (i == 4)
        {   
            sp.setTexture(texture[i]);
     

            sprite.push_back(sp);
            sprite[4].setOrigin({ 130.0f , 113.0f });
            sprite[4].setScale({ 0.07f, 0.07f });
            sprite[4].setPosition({ position.x + 95.0f , position.y + 105.0f });
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
        else if (i == 2)
        {
            sprite[2].setPosition({ position.x + xOffset , position.y + 85.0f});
        }
        else if (i == 3)
            sprite[3].setPosition({ position.x + 35.0f , position.y + 108.0f });
        else if (i == 4)
            sprite[4].setPosition({ position.x + 50.0f , position.y + 108.0f });
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

void PlayerParts::rotateWheels()
{

}
