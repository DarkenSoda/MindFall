#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <string>
#include <memory>

struct BossLayer {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f offset;
    bool isAnimated;

    BossLayer()
        : offset(0.f, 0.f)
        , isAnimated(false)
        , sprite(texture)
    {
    }
};

class Boss {
private:
    b2Body* body;
    b2World* world;
    sf::Vector2f position;
    sf::Vector2f size;
    
    std::vector<BossLayer> layers;
    int pupilLayerIndex;
    
    float pupilCircleRadius;
    float pupilAngle;
    sf::Vector2f pupilOffset;    
    sf::Vector2f playerPosition;
    
    float moveSpeed;
    float movementDirection;
    float leftBound;
    float rightBound;

    const float SCALE = 30.f;

public:
    Boss(b2World* world, sf::Vector2f startPosition, sf::Vector2f bossSize);
    ~Boss();

    void addLayer(const std::string& texturePath, sf::Vector2f offset = sf::Vector2f(0.f, 0.f), bool isAnimated = false);
    void setPupilLayer(int layerIndex, float circleRadius = 10.f);
    void setPlayerPosition(sf::Vector2f playerPos);
    void setBounds(float left, float right);
    void setMoveSpeed(float speed);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const { return position; }
    b2Body* getBody() const { return body; }
};