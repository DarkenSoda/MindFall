#include "Player.h"
#include <iostream>
using namespace std;

void Player::animations()
{
    animator
        .SetAnimation("idle", std::make_unique<Animation>("assets/player/idle.png", 0.125f, 8))
        .SetAnimation("left", std::make_unique<Animation>("assets/player/walk_left.png", 0.125f, 8))
        .SetAnimation("right", std::make_unique<Animation>("assets/player/walk_right.png", 0.125f, 8));
}

Player::Player(sf::Vector2f position, float moveSpeed)
{
    this->moveSpeed = moveSpeed;
    this->position = position;
    animations();
    rage = 0;
    multilyer = 5.5f;;
    rageDirection = false;
    maximumRage = 100.0f;
    minimumRage = 0.0f;
    timeToEmptyRage = 5.0f;
    rageDownCooldown = 0.0f;
}

void Player::move(PlayerCommand cmd, float deltaTime)
{
    std::string s;
    switch (cmd)
    {
    case PlayerCommand::moveLeft:
        position.x -= moveSpeed * deltaTime;
        s = "left";
        break;

    case PlayerCommand::moveRight:
        position.x += moveSpeed * deltaTime;
        s = "right";
        break;

    default:
        s = "idle";
        break;
    }
    animator.PlayAnimation(s, deltaTime, position);
}

void Player::setPlayerPosition(sf::Vector2f position)
{
    this->position = position;
}

sf::Vector2f Player::getPlayerPosition()
{
    return position;
}

sf::FloatRect Player::getGlobalBound()
{
    return animator.CurrentAnimaton().getGlobalBounds();
}

sf::Sprite& Player::CurrentAnimaton()
{
    return animator.CurrentAnimaton();
}

void Player::update(float deltatime)
{
    if (rage>=maximumRage)
    {
        rageDirection = true;
        rage = 100.0f;
        cout << "random Event";
    }
    if (rage <= minimumRage)
    {
        rageDirection = false;
        multilyer = 2.5f;
        rage = 0.0f;
    }
    if (!rageDirection)
    {
        rage += deltatime * multilyer;
    }
    else
    {
        float interval = 100.0f / timeToEmptyRage;
        rageDownCooldown += deltatime;
        if (rageDownCooldown >= 1.0f)
        {
            rage -= interval;
            rageDownCooldown = 0.0f;
        }
    }
   

    cout << rage<<endl;
}