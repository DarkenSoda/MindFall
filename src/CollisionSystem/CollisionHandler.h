#pragma once

#include <box2d/box2d.h>

class CollisionHandler : public b2ContactListener {
public:
    CollisionHandler();

    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
};