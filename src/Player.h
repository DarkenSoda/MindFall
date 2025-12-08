#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "AnimationSystem/Animator.h"
#include "AnimationSystem/Animation.h"
#include "PlayerCommand.h"
#include "InputHandler.h"

class Player {
private:
    sf::Vector2f position;
    Animator animator;
    float moveSpeed;

    void animations();  
public:
    Player(sf::Vector2f position, float moveSpeed);

    void move(PlayerCommand cmd, float deltaTime);

    void setPlayerPosition(sf::Vector2f position);

    sf::Vector2f getPlayerPosition();

    sf::FloatRect getGlobalBound();

    sf::Sprite& CurrentAnimaton();
};
