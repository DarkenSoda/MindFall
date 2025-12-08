#include "WorldSpawner.h"

WorldSpawner::WorldSpawner(b2World* world) 
    : world(world)
    , spawnTimer(0.f)
    , spawnInterval(2.0f)
    , randomGenerator(std::random_device{}())
    , spawnTypeReversed(false)
{

    sf::Texture texture1;
    sf::Texture texture2;

    if(!texture1.loadFromFile("assets/prop1.png") || !texture2.loadFromFile("assets/prop2.png")) {
        std::cerr << "Error loading collectible textures!" << std::endl;
    }

    texture1.setSmooth(true);
    texture2.setSmooth(true);

    if(!texture1.generateMipmap() || !texture2.generateMipmap()) {
        std::cerr << "Error generating mipmaps for collectible textures!" << std::endl;
    }

    collectiblePrototypes = {
        {
            texture1,
            texture2,
            CollectibleType::Score,
            70.f,
            28.f
        },
        {
            texture1,
            texture2,
            CollectibleType::Damage,
            30.f,
            28.f
        }
    };
}

void WorldSpawner::update(float deltaTime, float screenWidth) {
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;

        CollectiblePrototype* prototypeToSpawn = selectCollectibleToSpawn();
        
        if (prototypeToSpawn != nullptr) {
            std::uniform_real_distribution<float> xDist(50.f, screenWidth - 50.f);
            float randomX = xDist(randomGenerator);
            
            sf::Vector2f spawnPosition(randomX, -5.f);
            sf::Vector2f downwardVelocity(0.f, 200.f);

            // Create new collectible using prototype reference
            auto newCollectible = std::make_unique<Collectible>(
                *prototypeToSpawn, world, spawnPosition, downwardVelocity,
                spawnTypeReversed ? CollectibleType::Damage : CollectibleType::Score
            );
            
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

void WorldSpawner::addCollectiblePrototype(CollectiblePrototype prototype) {
    collectiblePrototypes.push_back(std::move(prototype));
}

CollectiblePrototype* WorldSpawner::selectCollectibleToSpawn() {
    if (collectiblePrototypes.empty()) {
        return nullptr;
    }

    float totalPercentage = 0.f;
    for (const auto& prototype : collectiblePrototypes) {
        totalPercentage += prototype.spawnPercentage;
    }

    std::uniform_real_distribution<float> dist(0.f, totalPercentage);
    float randomValue = dist(randomGenerator);

    float cumulativePercentage = 0.f;
    for (auto& prototype : collectiblePrototypes) {
        cumulativePercentage += prototype.spawnPercentage;
        if (randomValue <= cumulativePercentage) {
            return &prototype;
        }
    }

    return &collectiblePrototypes[0];
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
