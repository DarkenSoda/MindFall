#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer()
    : speed(0.0f), texture(), sprite1(texture), sprite2(texture)
{
}

bool ParallaxLayer::load(const std::string& file, float scrollSpeed)
{
    if (!texture.loadFromFile(file))
        return false;

    texture.setRepeated(true);

    sprite1.setTexture(texture);
    sprite2.setTexture(texture);

    sprite2.setPosition({ static_cast<float>(texture.getSize().x), 0.0f });

    speed = scrollSpeed;
    return true;
}

void ParallaxLayer::update(float dt)
{
    sprite1.move({ -speed * dt, 0 });
    sprite2.move({ -speed * dt, 0 });

    float width = texture.getSize().x;

    if (sprite1.getPosition().x <= -width)
        sprite1.setPosition({ sprite2.getPosition().x + width, 0.0f });

    if (sprite2.getPosition().x <= -width)
        sprite2.setPosition({ sprite1.getPosition().x + width, 0.0f });
}

void ParallaxLayer::draw(sf::RenderWindow& window)
{
    window.draw(sprite1);
    window.draw(sprite2);
}
