#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "../AnimationSystem/Animator.h"
#include "../AnimationSystem/Animation.h"
#include "PlayerCommand.h"
#include "InputHandler.h"
#include "PlayerParts.h"
#include "../EntityTypes.h"
#include "../EntityData.h"

class Player {
private:
    b2Body* body;
    b2World* world;
    sf::Vector2f position;
    sf::Vector2f size;
    Animator animator;
    PlayerParts* playerParts;
    float moveSpeed;
    float rage;
    float multiplier;
    bool rageDirection;
    float maximumRage;
    float minimumRage;
    float timeToEmptyRage;
    float rageDownCooldown;
    int lives;
	string state;
    EntityData entityData;
    
    // Invulnerability system
    bool isInvulnerable;
    float invulnerabilityTimer;
    float invulnerabilityDuration;
    float flashTimer;
    float flashInterval;
    bool isFlashVisible;
    
    float windowWidth;
    float windowHeight;
    
    const float SCALE = 30.f;
    
    void animations();  
public:
    Player(b2World* world, sf::Vector2f position, sf::Vector2f size, float moveSpeed, float windowWidth, float windowHeight);

    ~Player();

    void move(PlayerCommand cmd, float deltaTime);

    void setPlayerPosition(sf::Vector2f position);

    sf::Vector2f getPlayerPosition();

    float getRage();

    sf::FloatRect getGlobalBound();

    void drawPlayer(sf::RenderWindow& window);

    void update(float deltatime);

    int getLives() const;

    void takeDamage(int damage = 1);
    
    bool getIsInvulnerable() const { return isInvulnerable; }

    float getRageInterval() const;
    
    b2Body* getBody() const { return body; }
};
