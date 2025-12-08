#include "Collectible.h"


Collectible::Collectible(const CollectiblePrototype& prototypeRef, b2World* world,
    sf::Vector2f startPosition, sf::Vector2f initialVelocity, CollectibleType type)
    : type(type)
    , spawnPercentage(prototypeRef.spawnPercentage)
    , radius(prototypeRef.radius)
    , body(nullptr)
    , world(world)
    , position(startPosition)
    , isActive(true)
    , usingFirstTexture(type == CollectibleType::Score)
    , prototype(&prototypeRef)
    , sprite(prototypeRef.type == CollectibleType::Score ? prototypeRef.texture1 : prototypeRef.texture2)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    sprite.setScale({ 0.05f, 0.05f });

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(startPosition.x, startPosition.y);
    bodyDef.linearVelocity.Set(initialVelocity.x, initialVelocity.y);
    body = world->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = prototypeRef.radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    sprite.setPosition(startPosition);
}

Collectible::~Collectible() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void Collectible::update(float deltaTime) {
    if (!isActive || !body) return;

    b2Vec2 bodyPos = body->GetPosition();
    position = sf::Vector2f(bodyPos.x, bodyPos.y);
    sprite.setPosition(position);

    // if (position.y > 1200.f) {
    //     isActive = false;
    // }
}

void Collectible::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);

        // debug draw collider wireframe
        sf::CircleShape debugCircle(radius);
        debugCircle.setOrigin({ radius, radius });
        debugCircle.setPosition(position);
        debugCircle.setFillColor(sf::Color::Transparent);
        debugCircle.setOutlineColor(sf::Color::Blue);
        debugCircle.setOutlineThickness(1.f);
        window.draw(debugCircle);
    }
}

void Collectible::switchTexture() {
    if (!prototype) return;
    
    if (usingFirstTexture) {
        sprite.setTexture(prototype->texture2);
    }
    else {
        sprite.setTexture(prototype->texture1);
    }
    usingFirstTexture = !usingFirstTexture;
}

void Collectible::setPosition(sf::Vector2f pos) {
    position = pos;
    sprite.setPosition(position);
    if (body) {
        body->SetTransform(b2Vec2(position.x, position.y), 0.f);
    }
}
