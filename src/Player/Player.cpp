#include "Player.h"
#include <iostream>
using namespace std;

void Player::animations()
{
    animator
        .SetAnimation("idle", std::make_unique<Animation>("assets/player/happy.png", 0.125f, 1))
        .SetAnimation("left", std::make_unique<Animation>("assets/player/hand.png", 0.125f, 1))
        .SetAnimation("right", std::make_unique<Animation>("assets/player/plate.png", 0.125f, 1));
}

Player::Player()
{
    this->moveSpeed = 250.0f;
    this->position = {200.0f, 800.0f};
    animations();
    rage = 1.0f;
    multilyer = 20.5f;;
    rageDirection = false;
    maximumRage = 100.0f;
    minimumRage = 0.0f;
    timeToEmptyRage = 5.0f;
    rageDownCooldown = 1.0f;
    playerParts = new PlayerParts(position);
}

Player::Player(sf::Vector2f position, float moveSpeed)
{
    this->moveSpeed = moveSpeed;
    this->position = position;
    animations();
    rage = 1.0f;
    multilyer = 20.5f;;
    rageDirection = false;
    maximumRage = 100.0f;
    minimumRage = 0.0f;
    timeToEmptyRage = 5.0f;
    rageDownCooldown = 1.0f;
    playerParts = new PlayerParts(position);
}

Player::~Player()
{
    delete playerParts;
}

void Player::move(PlayerCommand cmd, float deltaTime)
{
    std::string s;
    switch (cmd)
    {
    case PlayerCommand::moveLeft:
        position.x -= moveSpeed * deltaTime;
        playerParts->updateParts(position);
        s = "left";
        break;

    case PlayerCommand::moveRight:

        position.x += moveSpeed * deltaTime;
        playerParts->updateParts(position);
        s = "right";
        break;

    default:
        s = "idle";
        playerParts->updateParts(position);
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

float Player::getRage()
{
    return rage;
}

sf::FloatRect Player::getGlobalBound()
{
    return animator.CurrentAnimaton().getGlobalBounds();
}

void Player::drawPlayer(sf::RenderWindow& window)
{
    playerParts->drawBehindParts(window);
    window.draw(animator.CurrentAnimaton());
    playerParts->drawFrontParts(window);
}

void Player::update(float deltatime)
{
    if (rage>=maximumRage)
    {
        rageDirection = true;
        rage = 100.0f;
    }
    if (rage <= minimumRage)
    {
        rageDirection = false;
        multilyer = 20.5f;
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
}