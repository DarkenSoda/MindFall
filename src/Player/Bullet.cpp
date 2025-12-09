#include "Bullet.h"
#include <cmath>

Bullet::Bullet(const sf::Texture& texture, sf::Vector2f startPos, sf::Vector2f direction) {
    sprite.setTexture(texture);
    sprite.setPosition(startPos);

    // Normalize direction
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;

    velocity = direction * speed;

    // Optional: rotate sprite to match direction
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
    sprite.setRotation(sf::degrees(angle));
}

void Bullet::update(float deltaTime) {
    sprite.move(velocity * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}
