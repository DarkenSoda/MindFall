#pragma once
#include <SFML/Graphics.hpp>

class RageBar {
public:
    RageBar(const sf::Texture& texture, int maxRage);
    ~RageBar();

    void setRage(int currentRage);
    void render(sf::RenderWindow& window);

private:    
    sf::Sprite sprite;
    sf::Texture texture;
    int maxRage;
    int frameWidth;
    int frameHeight;
};