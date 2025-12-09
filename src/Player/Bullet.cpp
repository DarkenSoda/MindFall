#include "Bullet.h"

Bullet::Bullet(const sf::Texture& texture, float startX, float startY)
    : sprite(texture)
{
    speed = 15.f;

    sprite.setScale({ 0.05f, 0.05f });
    sf::Vector2f size = sprite.getGlobalBounds().size;
    sprite.setPosition({ startX - (size.x / 2.f), startY });
}

void Bullet::update() {
    sprite.move({ 0.f, -speed });
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}