#pragma once
#include <SFML/Graphics.hpp>

class HealthBar {
public:
    // Constructor: 
    // texture: pointer to the loaded image strip
    // maxHealth: usually 6 in your case
    HealthBar(const sf::Texture& texture, int maxHealth);

    // Call this whenever player gets hit or healed
    void setHealth(int currentHealth);

    void render(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    int maxHP;
    int frameWidth;  // How wide is ONE heart/icon?
    int frameHeight; // How tall is the image?
};