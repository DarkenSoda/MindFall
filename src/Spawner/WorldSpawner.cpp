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

    if(!texture1.loadFromFile("assets/green_smile.png") || !texture2.loadFromFile("assets/red_smile.png")) {
        std::cerr << "Error loading collectible textures!" << std::endl;
    }

    texture1.setSmooth(true);
    texture2.setSmooth(true);

    if(!texture1.generateMipmap() || !texture2.generateMipmap()) {
        std::cerr << "Error generating mipmaps for collectible textures!" << std::endl;
    }
    sf::Texture texture3;
    sf::Texture texture4;

    if(!texture3.loadFromFile("assets/green_sad.png") || !texture4.loadFromFile("assets/red_sad.png")) {
        std::cerr << "Error loading collectible textures!" << std::endl;
    }

    texture1.setSmooth(true);
    texture2.setSmooth(true);

    if(!texture3.generateMipmap() || !texture4.generateMipmap()) {
        std::cerr << "Error generating mipmaps for collectible textures!" << std::endl;
    }

    CollectiblePrototype scorePrototype;
    scorePrototype.texture1 = texture1;
    scorePrototype.texture2 = texture2;
    scorePrototype.type = CollectibleType::Score;
    scorePrototype.spawnPercentage = 70.f;
    scorePrototype.radius = 28.f;

    CollectiblePrototype damagePrototype;
    damagePrototype.texture1 = texture3;
    damagePrototype.texture2 = texture4;
    damagePrototype.type = CollectibleType::Damage;
    damagePrototype.spawnPercentage = 30.f;
    damagePrototype.radius = 28.f;

    collectiblePrototypes.push_back(scorePrototype);
    collectiblePrototypes.push_back(damagePrototype);
}

void WorldSpawner::update(float deltaTime, float screenWidth) {
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;

        CollectiblePrototype* prototypeToSpawn = selectCollectibleToSpawn();
        
        if (prototypeToSpawn != nullptr) {
            std::uniform_real_distribution<float> xDist(80.f, screenWidth - 80.f);
            float randomX = xDist(randomGenerator);
            
            sf::Vector2f spawnPosition(randomX, -5.f);
            sf::Vector2f downwardVelocity(0.f, 200.f);

            // Create new collectible using prototype reference
            // The type will be handled by the collectible based on spawnTypeReversed
            auto newCollectible = std::make_unique<Collectible>(
                *prototypeToSpawn, world, spawnPosition, downwardVelocity, spawnTypeReversed
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

void WorldSpawner::switchAllCollectible() {
    spawnTypeReversed = !spawnTypeReversed;
    
    for (auto& collectible : collectibles) {
        collectible->switchTexture();
    }
}

void WorldSpawner::resetType() {
    spawnTypeReversed = false;

    for (auto& collectible : collectibles) {
        const CollectiblePrototype* prototype = collectible->getPrototype();
        if (prototype && collectible->getType() != prototype->type) {
            collectible->switchTexture();
        }
    }
}

void WorldSpawner::resetSpawner()
{
    collectibles.clear();
	spawnTypeReversed = false;
}
