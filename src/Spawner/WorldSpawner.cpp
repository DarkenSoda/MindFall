#include "WorldSpawner.h"

WorldSpawner::WorldSpawner(b2World* world) 
    : world(world)
    , spawnTimer(0.f)
    , spawnInterval(2.0f)
    , randomGenerator(std::random_device{}())
    , spawnTypeReversed(false)
{
}

void WorldSpawner::update(float deltaTime, float screenWidth) {
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;

        Collectible* prototypeToSpawn = selectCollectibleToSpawn();
        
        if (prototypeToSpawn != nullptr) {
            std::uniform_real_distribution<float> xDist(50.f, screenWidth - 50.f);
            float randomX = xDist(randomGenerator);
            
            sf::Vector2f spawnPosition(randomX, -5.f);
            sf::Vector2f downwardVelocity(0.f, 200.f);

            
            auto newCollectible = prototypeToSpawn->clone(world, spawnPosition, downwardVelocity, spawnTypeReversed ? CollectibleType::Damage : CollectibleType::Score);
            
            collectibles.push_back(std::move(newCollectible));
        }
    }

    for (auto& collectible : collectibles) {
        collectible->update(deltaTime);
    }

    removeInactiveCollectibles();
}

void WorldSpawner::draw(sf::RenderWindow& window) {
    for (auto& collectible : collectibles) {
        collectible->draw(window);
    }
}

void WorldSpawner::addCollectiblePrototype(std::unique_ptr<Collectible> prototype) {
    collectiblePrototypes.push_back(std::move(prototype));
}

Collectible* WorldSpawner::selectCollectibleToSpawn() {
    if (collectiblePrototypes.empty()) {
        return nullptr;
    }

    float totalPercentage = 0.f;
    for (const auto& prototype : collectiblePrototypes) {
        totalPercentage += prototype->getSpawnPercentage();
    }

    std::uniform_real_distribution<float> dist(0.f, totalPercentage);
    float randomValue = dist(randomGenerator);

    float cumulativePercentage = 0.f;
    for (const auto& prototype : collectiblePrototypes) {
        cumulativePercentage += prototype->getSpawnPercentage();
        if (randomValue <= cumulativePercentage) {
            return prototype.get();
        }
    }

    return collectiblePrototypes[0].get();
}

static bool isCollectibleInactive(const std::unique_ptr<Collectible>& collectible) {
    return !collectible->getIsActive();
}

void WorldSpawner::removeInactiveCollectibles() {
    collectibles.erase(
        std::remove_if(collectibles.begin(), collectibles.end(), isCollectibleInactive),
        collectibles.end()
    );
}
