#include "CollisionHandler.h"
#include "../Spawner/Collectible.h"
#include "../BossSystem/BossProjectile.h"
#include "../BossSystem/BossLaser.h"
#include "../BossSystem/Boss.h"

CollisionHandler::CollisionHandler(GameManager* gameManager) {
    this->gameManager = gameManager;
}

void CollisionHandler::BeginContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (!fixtureA || !fixtureB) return;

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Get entity data from UserData
    EntityData* dataA = reinterpret_cast<EntityData*>(bodyA->GetUserData().pointer);
    EntityData* dataB = reinterpret_cast<EntityData*>(bodyB->GetUserData().pointer);

    if (!dataA || !dataB) return;

    // Handle collisions based on entity types
    EntityType type1 = dataA->type;
    EntityType type2 = dataB->type;
    EntityData* data1 = dataA;
    EntityData* data2 = dataB;
    b2Fixture* fixture1 = fixtureA;
    b2Fixture* fixture2 = fixtureB;

    // Ensure consistent ordering: type1 should be Player/Boss/Ground
    if (static_cast<int>(type1) > static_cast<int>(type2)) {
        std::swap(type1, type2);
        std::swap(data1, data2);
        std::swap(fixture1, fixture2);
    }

    switch (type1) {
    case EntityType::PLAYER:
        switch (type2) {
        case EntityType::COLLECTIBLE:
            handlePlayerCollectible(data1, data2);
            break;
        case EntityType::DAMAGE:
            handlePlayerDamage(data1, data2);
            break;
        case EntityType::PROJECTILE:
            handlePlayerProjectile(data1, data2);
            break;
        case EntityType::LASER:
            handlePlayerLaser(data1, data2);
            break;
        default:
            break;
        }
        break;

    case EntityType::BOSS:
        switch (type2) {
        case EntityType::BULLET:
            if (!fixture1->IsSensor()) {
                handleBossHit(data1, data2);
            }
            break;
        }
        break;

    case EntityType::GROUND:
        switch (type2) {
        case EntityType::COLLECTIBLE:
            handleGroundCollectible(data1, data2);
            break;
        case EntityType::DAMAGE:
            handleGroundDamage(data1, data2);
            break;
        case EntityType::PROJECTILE:
            handleGroundProjectile(data1, data2);
            break;
        }
        break;

    default:
        break;
    }
}

void CollisionHandler::EndContact(b2Contact* contact) {
}


// ==================== Player Collision Handlers ====================

void CollisionHandler::handlePlayerCollectible(EntityData* playerData, EntityData* collectibleData) {
    Collectible* collectible = static_cast<Collectible*>(collectibleData->objectPtr);
    if (collectible) {
        sf::Vector2f pos = collectible->getPosition();
        collectible->setActive(false);

        gameManager->addScore();
        gameManager->spawnParticleAt(pos, sf::Color::Green);
    }
}

void CollisionHandler::handlePlayerDamage(EntityData* playerData, EntityData* damageData) {
    Collectible* collectible = static_cast<Collectible*>(damageData->objectPtr);
    if (collectible) {
        sf::Vector2f pos = collectible->getPosition();
        collectible->setActive(false);

        gameManager->applyDamageToPlayer();
        gameManager->spawnParticleAt(pos, sf::Color::Red);
    }
}

void CollisionHandler::handlePlayerProjectile(EntityData* playerData, EntityData* projectileData) {
    BossProjectile* projectile = static_cast<BossProjectile*>(projectileData->objectPtr);
    if (projectile) {
        sf::Vector2f pos = projectile->getPosition();
        projectile->setActive(false);

        gameManager->applyDamageToPlayer();
        gameManager->spawnParticleAt(pos, sf::Color::Magenta);
    }
}

void CollisionHandler::handlePlayerLaser(EntityData* playerData, EntityData* laserData) {
    gameManager->applyDamageToPlayer();
}


// ==================== Boss Collision Handlers ====================

void CollisionHandler::handleBossHit(EntityData* bossData, EntityData* collectibleData) {
    Bullet* bullet = static_cast<Bullet*>(collectibleData->objectPtr);
    if (bullet) {
        sf::Vector2f pos = bullet->getPosition();
        bullet->setActive(false);

        Boss* boss = static_cast<Boss*>(bossData->objectPtr);
        gameManager->applyDamageToBoss();
        gameManager->spawnParticleAt(pos, sf::Color(128, 128, 128)); // gray
    }
}

// ==================== Ground Collision Handlers ====================

void CollisionHandler::handleGroundCollectible(EntityData* groundData, EntityData* collectibleData) {
    Collectible* collectible = static_cast<Collectible*>(collectibleData->objectPtr);
    if (collectible) {
        sf::Vector2f pos = collectible->getPosition();
        collectible->setActive(false);

        gameManager->spawnParticleAt(pos, sf::Color::Green);
    }
}

void CollisionHandler::handleGroundDamage(EntityData* groundData, EntityData* damageData) {
    Collectible* collectible = static_cast<Collectible*>(damageData->objectPtr);
    if (collectible) {
        sf::Vector2f pos = collectible->getPosition();
        collectible->setActive(false);

        gameManager->spawnParticleAt(pos, sf::Color::Red);
    }
}

void CollisionHandler::handleGroundProjectile(EntityData* groundData, EntityData* projectileData) {
    BossProjectile* projectile = static_cast<BossProjectile*>(projectileData->objectPtr);
    if (projectile) {
        sf::Vector2f pos = projectile->getPosition();
        projectile->setActive(false);

        gameManager->spawnParticleAt(pos, sf::Color::Magenta);
    }
}