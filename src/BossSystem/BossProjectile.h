#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class BossProjectile {
private:
    b2Body* body;
    b2World* world;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
    
    sf::CircleShape circle;
    sf::Texture texture;
    
    float windowWidth;
    float windowHeight;
    
    const float SCALE = 30.f;
    
    bool active;

public:
    BossProjectile(b2World* world, sf::Vector2f startPosition, sf::Vector2f startVelocity, 
                   float radius, float windowWidth, float windowHeight);
    ~BossProjectile();
    
    bool loadTexture(const std::string& texturePath);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    void checkBorderReflection();
    
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    
    sf::Vector2f getPosition() const { return position; }
    float getRadius() const { return radius; }
    b2Body* getBody() const { return body; }
};
