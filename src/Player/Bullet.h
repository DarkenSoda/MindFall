#pragma once
#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = 500.f;

public:
    Bullet(const sf::Texture& texture, sf::Vector2f startPos, sf::Vector2f direction);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
};
