#include "Player.h"
#include <iostream>
using namespace std;

void Player::animations()
{
    animator
        .SetAnimation("idle", std::make_unique<Animation>("assets/player/happy.png", 0.125f, 1))
        .SetAnimation("stress", std::make_unique<Animation>("assets/player/stressed.png", 0.125f, 1));
}

Player::Player(sf::Vector2f position, float moveSpeed)
{
    this->moveSpeed = moveSpeed;
    this->position = position;
    animations();
    rage = 1.0f;
    multilyer = 20.5f;
    rageDirection = false;
    maximumRage = 100.0f;
    minimumRage = 0.0f;
    timeToEmptyRage = 5.0f;
    rageDownCooldown = 1.0f;
    playerParts = new PlayerParts(position);
    lives = 6;
	state = "idle";
}

Player::~Player()
{
    delete playerParts;
}

void Player::move(PlayerCommand cmd, float deltaTime)
{
    switch (cmd)
    {
    case PlayerCommand::moveLeft:
        position.x -= moveSpeed * deltaTime;
        playerParts->updateParts(position);
        break;

    case PlayerCommand::moveRight:

        position.x += moveSpeed * deltaTime;
        playerParts->updateParts(position);
        break;

    default:
        playerParts->updateParts(position);
        break;
    }
    animator.PlayAnimation(state, deltaTime, position);
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

float Player::getRageInterval() const {
    // Map rage to the nearest multiple of 20
    return std::clamp(static_cast<int>(rage / 20) * 20, 0, 100);
}

int Player::getLives() const
{
    return lives;
}

void Player::takeDamage(int damage)
{
    lives -= damage;
    if (lives < 0)
        lives = 0;
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
    if (rage >= maximumRage)
    {
        rageDirection = true;
        rage = 100.0f;
        state = "stress";
		playerParts->setRotationSpeed(20.0f);
    }
    if (rage <= minimumRage)
    {
        rageDirection = false;
        multilyer = 20.5f;
        rage = 0.0f;
        state = "idle";
        playerParts->setRotationSpeed(10.0f);
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