#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "../EntityTypes.h"
#include "../EntityData.h"

enum class LaserState {
    Inactive,
    Expanding,
    Active,
    Shrinking
};

class BossLaser {
private:
    b2Body* body;
    b2World* world;
    b2Fixture* laserFixture;
    
    sf::Vector2f position;
    float currentWidth;
    float maxWidth;
    float height;
    
    sf::RectangleShape laserShape;
    sf::Texture texture;
    
    LaserState state;
    float stateTimer;
    
    float expandDuration;
    float activeDuration;
    float shrinkDuration;
    
    float windowWidth;
    float windowHeight;
    
    const float SCALE = 30.f;
    EntityData entityData;
    
    void updateCollider();

public:
    BossLaser(b2World* world, float maxWidth, float height, float windowWidth, float windowHeight);
    ~BossLaser();
    
    bool loadTexture(const std::string& texturePath);
    void activate(sf::Vector2f bossPosition);
    void update(float deltaTime);
    void updatePosition(sf::Vector2f bossPosition);
    void render(sf::RenderWindow& window);
    
    bool isActive() const { return state != LaserState::Inactive; }
    LaserState getState() const { return state; }
    
    void setExpandDuration(float duration) { expandDuration = duration; }
    void setActiveDuration(float duration) { activeDuration = duration; }
    void setShrinkDuration(float duration) { shrinkDuration = duration; }

	void laserReset();
    
    sf::Vector2f getPosition() const { return position; }
    b2Body* getBody() const { return body; }
};
