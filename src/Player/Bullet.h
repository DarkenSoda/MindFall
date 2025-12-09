#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(const sf::Texture& texture, float startX, float startY);

    void update();
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }

private:
    sf::Sprite sprite;
    float speed;
    bool active;
};