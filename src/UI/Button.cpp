#include "Button.h"

Button::Button(float x, float y, const sf::Texture& texture) : btn_sprite(texture), isHovered(false) {

    btn_sprite.setPosition({x, y});
    normalColor = sf::Color(255, 255, 255, 255);
    hoverColor = sf::Color(180, 180, 180, 255); // Dimmed color
    btn_sprite.setColor(normalColor);
}

bool Button::isClicked(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

    if (btn_sprite.getGlobalBounds().contains(mousePosF)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            return true;
        }
    }
    return false;
}

void Button::update(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

    if (btn_sprite.getGlobalBounds().contains(mousePosF)) {
        btn_sprite.setColor(hoverColor);
        isHovered = true;
    } else {
        btn_sprite.setColor(normalColor);
        isHovered = false;
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(btn_sprite);
}