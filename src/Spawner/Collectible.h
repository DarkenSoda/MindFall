#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include <iostream>
#include <memory>
#include "CollectibleData.h"

class Collectible {
private:
    const float SCALE = 30.f;
    
    sf::Sprite sprite;
    const CollectiblePrototype* prototype;

    b2Body* body;
    b2World* world;
    float radius;

    sf::Vector2f position;

    CollectibleType type;

    float spawnPercentage;
    bool isActive;
    bool usingFirstTexture;

public:
    Collectible(const CollectiblePrototype& prototypeRef, b2World* world, 
                sf::Vector2f startPosition, sf::Vector2f initialVelocity, bool isReversed);

    ~Collectible();

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void switchTexture();
    
    const CollectiblePrototype* getPrototype() const { return prototype; }
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
