#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include "Collectible.h"

class WorldSpawner {
private:
    std::vector<std::unique_ptr<Collectible>> collectibles;
    std::vector<CollectiblePrototype> collectiblePrototypes;

    b2World* world;
    float spawnTimer;
    float spawnInterval;
    bool spawnTypeReversed;
    std::mt19937 randomGenerator;

    CollectiblePrototype* selectCollectibleToSpawn();

public:
    WorldSpawner(b2World* world);
    void update(float deltaTime, float screenWidth);
    void draw(sf::RenderWindow& window);

    void addCollectiblePrototype(CollectiblePrototype prototype);
    std::vector<std::unique_ptr<Collectible>>& getCollectibles() { return collectibles; }

    void removeInactiveCollectibles();
    void switchAllCollectible();
};
