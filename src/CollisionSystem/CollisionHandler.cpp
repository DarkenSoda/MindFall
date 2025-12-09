#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {}

void CollisionHandler::BeginContact(b2Contact* contact) {
    std::cout << "=== COLLISION DETECTED ===" << std::endl;
    
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    if (fixtureA && fixtureB) {
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();
        

    }
    
    std::cout << "=========================" << std::endl;
}

void CollisionHandler::EndContact(b2Contact* contact) {
}