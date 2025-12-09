#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include "BossProjectile.h"
#include "BossAnimation.h"

class Boss {
private:
    b2Body* body;
    b2World* world;
    sf::Vector2f position;
    sf::Vector2f size;
    
    sf::Texture spriteSheet;
    sf::Sprite sprite;
    std::map<std::string, BossAnimation> animations;
    std::string currentAnimation;
    int currentFrame;
    float animationTimer;
    
    int frameWidth;
    int frameHeight;
    
    sf::Vector2f playerPosition;
    
    float moveSpeed;
    float movementDirection;
    float leftBound;
    float rightBound;
    
    float attackTimer;
    float attackInterval;
    std::vector<std::unique_ptr<BossProjectile>> projectiles;
    float windowWidth;
    float windowHeight;

    const float SCALE = 30.f;
    
    void updateAnimation(float deltaTime);
    void updateAttacks(float deltaTime);
    void performAttack();
    void projectileAttack();

public:
    Boss(b2World* world, sf::Vector2f startPosition, sf::Vector2f bossSize, float windowWidth, float windowHeight);
    ~Boss();

    bool loadSpriteSheet(const std::string& texturePath, int frameWidth, int frameHeight);
    void addAnimation(const std::string& name, int row, int frameCount, float switchTime = 0.1f, bool loop = true);
    void playAnimation(const std::string& name);
    
    void setPlayerPosition(sf::Vector2f playerPos);
    void setBounds(float left, float right);
    void setMoveSpeed(float speed);
    void setAttackInterval(float interval);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const { return position; }
    b2Body* getBody() const { return body; }
};