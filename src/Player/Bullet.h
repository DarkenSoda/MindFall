#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "../EntityTypes.h"
#include "../EntityData.h"

class Bullet {
public:
    Bullet(b2World* world, const sf::Texture& texture, float startX, float startY, float bulletWidth, float bulletHeight);
    ~Bullet();
    
    // Delete copy operations
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
    
    // Define move operations
    Bullet(Bullet&& other) noexcept;
    Bullet& operator=(Bullet&& other) noexcept;

    void update();
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }

    b2Body* getBody() const { return body; }
    sf::Vector2f getPosition() const { return position; }

private:
    b2Body* body;
    b2World* world;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f size;
    float speed;
    bool active;
    EntityData entityData;
    
    static constexpr float SCALE = 30.f;
};