#include "BossProjectile.h"
#include <iostream>

BossProjectile::BossProjectile(b2World* world, sf::Vector2f startPosition, sf::Vector2f startVelocity,
    float radius, float windowWidth, float windowHeight)
    : world(world)
    , position(startPosition)
    , velocity(startVelocity)
    , radius(radius)
    , windowWidth(windowWidth)
    , windowHeight(windowHeight)
    , body(nullptr)
    , active(true)
    , entityData(EntityType::PROJECTILE, this) {

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.gravityScale = 0.0f;
    bodyDef.position.Set(startPosition.x / SCALE, startPosition.y / SCALE);

    body = world->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = radius / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 1.0f;

    body->CreateFixture(&fixtureDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);

    body->SetLinearVelocity(b2Vec2(velocity.x / SCALE, velocity.y / SCALE));

    circle.setRadius(radius);
    circle.setOrigin({ radius, radius });
}

BossProjectile::~BossProjectile() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void BossProjectile::setTexture(const sf::Texture& texture) {
    circle.setTexture(&texture);
}

void BossProjectile::update(float deltaTime) {
    if (!active) return;

    b2Vec2 bodyPos = body->GetPosition();
    position = sf::Vector2f(bodyPos.x * SCALE, bodyPos.y * SCALE);

    b2Vec2 vel = body->GetLinearVelocity();
    velocity = sf::Vector2f(vel.x * SCALE, vel.y * SCALE);

    checkBorderReflection();

    circle.setPosition(position);
}

void BossProjectile::checkBorderReflection() {
    b2Vec2 vel = body->GetLinearVelocity();
    bool velocityChanged = false;

    if (position.x - radius <= 0.f && vel.x < 0.f) {
        vel.x = -vel.x;
        velocityChanged = true;
        position.x = radius;
    }
    else if (position.x + radius >= windowWidth && vel.x > 0.f) {
        vel.x = -vel.x;
        velocityChanged = true;
        position.x = windowWidth - radius;
    }

    if (velocityChanged) {
        body->SetLinearVelocity(vel);
    }
}

void BossProjectile::render(sf::RenderWindow& window) {
    if (!active) return;

    window.draw(circle);
}
