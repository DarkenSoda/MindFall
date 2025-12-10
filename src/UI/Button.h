#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:

    Button(float x, float y, const sf::Texture& texture);

    bool isClicked(const sf::RenderWindow& window);

    void update(const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    private:
    sf::Sprite btn_sprite;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool isHovered;

};
