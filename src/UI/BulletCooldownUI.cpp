#include "BulletCooldownUI.h"

BulletCooldownUI::BulletCooldownUI(const sf::Texture& baseTexture, const sf::Texture& grayTexture)
    : baseTexture(baseTexture), grayTexture(grayTexture), baseSprite(baseTexture), graySprite(grayTexture) {
    
    frameHeight = static_cast<int>(baseTexture.getSize().y);

    baseSprite.setScale(sf::Vector2f(0.125f, 0.125f));
    graySprite.setScale(sf::Vector2f(0.125f, 0.125f));
}

void BulletCooldownUI::setCooldown(float currentTime, float maxTime) {
    // Calculate the percentage of cooldown remaining (1.0 = full cooldown, 0.0 = ready)
    float cooldownPercent = currentTime / maxTime;

    // Calculate height of gray overlay based on cooldown
    int grayHeight = static_cast<int>(frameHeight * cooldownPercent);

    // Set texture rect for gray sprite (top portion only)
    graySprite.setTextureRect(sf::IntRect(
        sf::Vector2i(0, 0),
        sf::Vector2i(static_cast<int>(grayTexture.getSize().x), grayHeight)
    ));
}

void BulletCooldownUI::setPosition(sf::Vector2f position) {
    baseSprite.setPosition(position);
    graySprite.setPosition(position);
}

void BulletCooldownUI::render(sf::RenderWindow& window) {
    window.draw(baseSprite);
    window.draw(graySprite);
}
