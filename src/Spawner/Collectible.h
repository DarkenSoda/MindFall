#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include <iostream>
#include <memory>

enum class CollectibleType {
    Score,
    Damage
};

class Collectible {
private:
    sf::Sprite sprite;
    sf::Texture texture1;
    sf::Texture texture2;

    b2Body* body;
    b2World* world;
    float radius;

    sf::Vector2f position;

    CollectibleType type;

    float spawnPercentage;
    bool isActive;
    bool usingFirstTexture;

public:
    Collectible(const std::string& texture1Path, const std::string& texture2Path, 
                CollectibleType collectibleType, float spawnPercent, float colliderRadius = 20.f);

    std::unique_ptr<Collectible> clone(b2World* world, sf::Vector2f startPosition, sf::Vector2f initialVelocity, CollectibleType type) const;

    ~Collectible();

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void switchTexture();
    
    bool getIsActive() const { return isActive; }
    CollectibleType getType() const { return type; }
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return position; }
    float getSpawnPercentage() const { return spawnPercentage; }
    b2Body* getBody() const { return body; }
    float getRadius() const { return radius; }
    
    void setActive(bool active) { isActive = active; }
    void setPosition(sf::Vector2f pos);
};
