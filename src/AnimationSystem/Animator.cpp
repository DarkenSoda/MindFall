#include "Animator.h"
#include "Animation.h"

Animator::Animator()
{
}

Animator& Animator::SetAnimation(std::string key, std::unique_ptr<Animation> animation)
{
    animations[key] = std::move(animation);
    // Set first animation as current if currentAnimation is empty
    if (currentAnimation.empty()) {
        currentAnimation = key;
    }
    return *this;
}

void Animator::PlayAnimation(std::string key, float deltaTime, sf::Vector2f position)
{
    // Only update if the animation exists
    if (animations.find(key) != animations.end()) {
        currentAnimation = key;
        animations.at(key)->Play(deltaTime, position);
    }
}

sf::Sprite& Animator::CurrentAnimaton()
{
    // Safety check: if currentAnimation doesn't exist, use the first available
    if (animations.find(currentAnimation) == animations.end() && !animations.empty()) {
        currentAnimation = animations.begin()->first;
    }
    return animations.at(currentAnimation)->getSprite();
}

Animator::~Animator()
{
}
