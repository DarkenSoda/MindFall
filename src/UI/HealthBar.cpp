#include <iostream>
#include "HealthBar.h"
#include <algorithm>

HealthBar::HealthBar(const sf::Texture& textureRef, int maxHealth)
    : texture(textureRef), sprite(texture), maxHP(maxHealth)
{
    texture.setSmooth(true);
    texture.generateMipmap();

    sf::Vector2u textureSize = texture.getSize();

    frameWidth = textureSize.x / maxHealth;
    frameHeight = textureSize.y;

    float centerX = 100.0f;
    float bottomY = 1080.f - frameHeight+20.0f;
    sprite.setPosition(sf::Vector2f(centerX, bottomY));
    
    sprite.setScale(sf::Vector2f(0.6f, 0.6f));

    setHealth(maxHP);
    
    auto bounds = sprite.getGlobalBounds();
}

HealthBar::~HealthBar()
{
}

void HealthBar::setHealth(int currentHealth)
{
    int index = std::clamp(maxHP - currentHealth, 0, maxHP - 1);

    int left = index * frameWidth;

    sprite.setTextureRect(sf::IntRect(
        sf::Vector2i(left, 0),
        sf::Vector2i(frameWidth, frameHeight)
    ));
    
}

void HealthBar::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}