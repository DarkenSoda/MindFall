#pragma once

#include <box2d/box2d.h>
#include <iostream>
#include "../EntityTypes.h"

class CollisionHandler : public b2ContactListener {
public:
    CollisionHandler();

    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
};