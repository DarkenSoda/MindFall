#include "Animator.h"
#include "Animation.h"

Animator::Animator()
{
}

Animator& Animator::SetAnimation(std::string key, std::unique_ptr<Animation> animation)
{
    animations[key] = std::move(animation);
    return *this;
}

void Animator::PlayAnimation(std::string key, float deltaTime, sf::Vector2f position)
{
    currentAnimation = key;
    animations.at(key)->Play(deltaTime, position);
}

sf::Sprite& Animator::CurrentAnimaton()
{
    return animations.at(currentAnimation)->getSprite();
}

Animator::~Animator()
{
}
