#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include "BossProjectile.h"
#include "BossLaser.h"
#include "BossAnimation.h"

class Boss {
private:
    b2Body* body;
    b2World* world;
    sf::Vector2f position;
    sf::Vector2f size;
    
    sf::Texture idleTexture;
    sf::Texture laserTexture;
    sf::Texture projectileTexture;
    sf::Sprite sprite;
    std::map<std::string, BossAnimation> animations;
    std::string currentAnimation;
    int currentFrame;
    float animationTimer;
        
    float moveSpeed;
    float movementDirection;
    float leftBound;
    float rightBound;
    
    float attackTimer;
    float attackInterval;
    std::vector<std::unique_ptr<BossProjectile>> projectiles;
    BossLaser laser;
    float windowWidth;
    float windowHeight;

    std::vector<sf::Texture> attackTextures;
    
    int hp;

    const float SCALE = 30.f;
    
    void updateAnimation(float deltaTime);
    void updateAttacks(float deltaTime);
    void performAttack();
    void projectileAttack();
    void laserAttack();

public:
    Boss(b2World* world, sf::Vector2f startPosition, sf::Vector2f bossSize, float windowWidth, float windowHeight);
    ~Boss();

    bool loadSprites(const std::string& idlePath, const std::string& laserPath, const std::string& projectilePath);
    void addAnimation(const std::string& name, sf::Texture& texture, int frameCount, float switchTime = 0.1f, bool loop = true);
    void playAnimation(const std::string& name);
    
    void setBounds(float left, float right);
    void setMoveSpeed(float speed);
    void setAttackInterval(float interval);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    void takeDamage();
    
    int getHP() const { return hp; }
    sf::Vector2f getPosition() const { return position; }
    b2Body* getBody() const { return body; }
};