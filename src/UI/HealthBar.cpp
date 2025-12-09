#include <iostream>
#include "HealthBar.h"
#include <algorithm>

HealthBar::HealthBar(const sf::Texture& textureRef, int maxHealth)
    : texture(textureRef), sprite(texture), maxHP(maxHealth)
{
    // 1. Calculate the size of a SINGLE frame
    // We assume the frames are arranged horizontally
    sf::Vector2u textureSize = texture.getSize();

    frameWidth = textureSize.x / maxHealth; // Total Width / 6
    frameHeight = textureSize.y;

    std::cout << "[HealthBar] Initialized with texture: " << textureSize.x << "x" << textureSize.y << std::endl;
    std::cout << "[HealthBar] Frame size: " << frameWidth << "x" << frameHeight << std::endl;

    // 2. Set the position to bottom center of screen
    // Window is 1920x1080, so bottom center = (1920/2 - frameWidth/2, 1080 - frameHeight - 20 padding)
    float centerX = (1920.f / 2.f) - (frameWidth / 2.f);
    float bottomY = 1080.f - frameHeight - 20.f;
    sprite.setPosition(sf::Vector2f(centerX, bottomY));
    
    // 3. Reduce scale to make it smaller (from 2.0f to 1.0f)
    sprite.setScale(sf::Vector2f(1.0f, 1.0f));

    // 4. Initialize to show full health
    setHealth(maxHP);
    
    auto bounds = sprite.getGlobalBounds();
    std::cout << "[HealthBar] Position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
    std::cout << "[HealthBar] Sprite bounds: " << bounds.size.x << "x" << bounds.size.y << std::endl;
}

HealthBar::~HealthBar()
{
    std::cout << "[HealthBar] Destroyed" << std::endl;
}

void HealthBar::setHealth(int currentHealth)
{
    // Logic: 
    // If Health = 6, we want Index 0 (First image)
    // If Health = 5, we want Index 1 (Second image)
    // Formula: Index = MaxHP - CurrentHP

    int index = std::clamp(maxHP - currentHealth, 0, maxHP - 1);

    // Calculate the rectangle coordinates
    int left = index * frameWidth;

    // Apply the rectangle to the sprite - SFML 3.0 syntax with size parameter
    sprite.setTextureRect(sf::IntRect(
        sf::Vector2i(left, 0),
        sf::Vector2i(frameWidth, frameHeight)
    ));
    
    std::cout << "[HealthBar] Health set to: " << currentHealth << " (Index: " << index << ", Left: " << left << ")" << std::endl;
}

void HealthBar::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void HealthBar::debugInfo() const
{
    auto bounds = sprite.getGlobalBounds();
    auto texRect = sprite.getTextureRect();
    
    std::cout << "[HealthBar Debug]" << std::endl;
    std::cout << "  Position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
    std::cout << "  Global Bounds: " << bounds.position.x << ", " << bounds.position.y 
              << " - " << bounds.size.x << "x" << bounds.size.y << std::endl;
    std::cout << "  Texture Rect: " << texRect.position.x << ", " << texRect.position.y 
              << " - " << texRect.size.x << "x" << texRect.size.y << std::endl;
}