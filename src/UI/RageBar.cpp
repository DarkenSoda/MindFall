#include "RageBar.h"
#include <algorithm>

RageBar::RageBar(const sf::Texture& textureRef, int maxRage)
    : texture(textureRef), sprite(texture), maxRage(maxRage) {
    sf::Vector2u textureSize = texture.getSize();

    texture.setSmooth(true);
    texture.generateMipmap();

    frameWidth = textureSize.x / maxRage;
    frameHeight = textureSize.y;

    float centerX = 1600; // Adjust position as needed
    float bottomY = 1080.f - frameHeight +30.0f; // Place above health bar
    sprite.setPosition(sf::Vector2f(centerX, bottomY));

    sprite.setScale(sf::Vector2f(0.7f, 0.7f));

    setRage(0); // Initialize to 0 rage
}

RageBar::~RageBar() {}

void RageBar::setRage(int currentRage) {
    int index = std::clamp(currentRage / 20, 0, maxRage - 1); // Divide rage into 5 intervals

    int left = index * frameWidth;

    sprite.setTextureRect(sf::IntRect(
        sf::Vector2i(left, 0),
        sf::Vector2i(frameWidth, frameHeight)
    ));
}

void RageBar::render(sf::RenderWindow& window) {
    window.draw(sprite);
}