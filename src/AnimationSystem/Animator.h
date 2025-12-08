#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include "Animation.h"

class Animator {
private:
    std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
    std::string currentAnimation;

public:
    Animator();
    Animator& SetAnimation(std::string key, std::unique_ptr<Animation> animation);
    void PlayAnimation(std::string key, float deltaTime, sf::Vector2f position);
    sf::Sprite& CurrentAnimaton();
    ~Animator();
};
