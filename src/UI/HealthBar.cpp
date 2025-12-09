#include "HealthBar.h"
#include <algorithm> // For std::clamp

HealthBar::HealthBar(const sf::Texture& texture, int maxHealth)
    : sprite(texture), maxHP(maxHealth)
{
    // 1. Calculate the size of a SINGLE frame
    // We assume the frames are arranged horizontally
    sf::Vector2u textureSize = texture.getSize();

    frameWidth = textureSize.x / maxHealth; // Total Width / 6
    frameHeight = textureSize.y;

    // 2. Set the initial position on screen (Top Left corner)
    sprite.setPosition({ 10.f, 10.f });

    // 3. Initialize to show full health (Index 0)
    setHealth(maxHP);
}

void HealthBar::setHealth(int currentHealth) {
    // Logic: 
    // If Health = 6, we want Index 0 (First image)
    // If Health = 5, we want Index 1 (Second image)
    // Formula: Index = MaxHP - CurrentHP

    int index = maxHP - currentHealth;

    // Safety: Ensure index stays between 0 and 5 (so game doesn't crash)
    // SFML 3.0/C++17 std::clamp is great here, or manual checks
    if (index < 0) index = 0;
    if (index >= maxHP) index = maxHP - 1;

    // Calculate the rectangle coordinates
    // left = index * width
    int left = index * frameWidth;

    // Apply the rectangle to the sprite
    // SFML 3: IntRect({left, top}, {width, height})
    sprite.setTextureRect(sf::IntRect({ left, 0 }, { frameWidth, frameHeight }));
}

void HealthBar::render(sf::RenderWindow& window) {
    window.draw(sprite);
}