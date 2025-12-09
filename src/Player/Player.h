#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../AnimationSystem/Animator.h"
#include "../AnimationSystem/Animation.h"
#include "PlayerCommand.h"
#include "InputHandler.h"
#include "PlayerParts.h"

class Player {
private:
    sf::Vector2f position;
    Animator animator;
    PlayerParts* playerParts;
    float moveSpeed;
    float rage;
    float multilyer;
    bool rageDirection;
    float maximumRage;
    float minimumRage;
    float timeToEmptyRage;
    float rageDownCooldown;
    string state;
    int lives;

    void animations();  
public:
    Player(sf::Vector2f position, float moveSpeed);
    Player();

    

    ~Player();

    void move(PlayerCommand cmd, float deltaTime);

    void setPlayerPosition(sf::Vector2f position);

    sf::Vector2f getPlayerPosition();

    float getRage();

    sf::FloatRect getGlobalBound();

   void drawPlayer(sf::RenderWindow& window);

    void update(float deltatime);
};
