#include "Player.h"

void Player::animations()
{
    animator
        .SetAnimation("idle", std::make_unique<Animation>("assets/player/idle.png", 0.125f, 8))
        .SetAnimation("up", std::make_unique<Animation>("assets/player/walk_up.png", 0.125f, 8))
        .SetAnimation("down", std::make_unique<Animation>("assets/player/walk_down.png", 0.125f, 8))
        .SetAnimation("left", std::make_unique<Animation>("assets/player/walk_left.png", 0.125f, 8))
        .SetAnimation("right", std::make_unique<Animation>("assets/player/walk_right.png", 0.125f, 8));
}

Player::Player(sf::Vector2f position, float moveSpeed)
{
    this->moveSpeed = moveSpeed;
    this->position = position;
    animations();
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

    case PlayerCommand::moveUp:
        position.y -= moveSpeed * deltaTime;
        s = "up";
        break;

    case PlayerCommand::moveDown:
        position.y += moveSpeed * deltaTime;
        s = "down";
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
