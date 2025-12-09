#include "Button.h"

Button::Button(float x, float y, const sf::Texture& texture) : btn_sprite(texture) {

    btn_sprite.setPosition({x, y});
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



void Button::draw(sf::RenderWindow& window) {
    window.draw(btn_sprite);
}