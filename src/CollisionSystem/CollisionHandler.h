#pragma once

#include <box2d/box2d.h>
#include <iostream>
#include "../EntityTypes.h"
#include "../EntityData.h"
#include "../World/GameManager.h"

class CollisionHandler : public b2ContactListener {
private:
    GameManager* gameManager;

    // Player collision handlers
    void handlePlayerCollectible(EntityData* playerData, EntityData* collectibleData);
    void handlePlayerDamage(EntityData* playerData, EntityData* damageData);
    void handlePlayerProjectile(EntityData* playerData, EntityData* projectileData);
    void handlePlayerLaser(EntityData* playerData, EntityData* laserData);

    // Boss collision handlers
    void handleBossHit(EntityData* bossData, EntityData* collectibleData);

    // Ground collision handlers
    void handleGroundCollectible(EntityData* groundData, EntityData* collectibleData);
    void handleGroundDamage(EntityData* groundData, EntityData* damageData);
    void handleGroundProjectile(EntityData* groundData, EntityData* projectileData);

public:
    CollisionHandler(GameManager* gameManager);

    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
};