#pragma once
#include <SFML/Graphics.hpp>

class BulletCooldownUI {
public:
    BulletCooldownUI(const sf::Texture& baseTexture, const sf::Texture& grayTexture);
    
    void setCooldown(float currentTime, float maxTime);
    void setPosition(sf::Vector2f position);
    void render(sf::RenderWindow& window);

private:
    sf::Sprite baseSprite;
    sf::Sprite graySprite;
    sf::Texture baseTexture;
    sf::Texture grayTexture;
    int frameHeight;
};
