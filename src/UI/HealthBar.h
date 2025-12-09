#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class HealthBar {
public:
    HealthBar(const sf::Texture& texture, int maxHealth);
    ~HealthBar();

    void setHealth(int currentHealth);
    void render(sf::RenderWindow& window);
    

private:
    sf::Sprite sprite;
    sf::Texture texture;
    int maxHP;
    int frameWidth;
    int frameHeight;
};