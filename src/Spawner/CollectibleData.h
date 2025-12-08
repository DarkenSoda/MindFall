#pragma once

#include <SFML/Graphics.hpp>

enum class CollectibleType {
    Score,
    Damage
};

struct CollectiblePrototype {
    sf::Texture texture1;
    sf::Texture texture2;

    CollectibleType type;
    float spawnPercentage;
    float radius;
};