#include "Collectible.h"


Collectible::Collectible(const CollectiblePrototype& prototypeRef, b2World* world,
    sf::Vector2f startPosition, sf::Vector2f initialVelocity, bool isReversed)
    : spawnPercentage(prototypeRef.spawnPercentage)
    , radius(prototypeRef.radius)
    , body(nullptr)
    , world(world)
    , position(startPosition)
    , isActive(true)
    , prototype(&prototypeRef)
    , sprite(prototypeRef.texture1)
    , entityData(EntityType::COLLECTIBLE, this)
{
    if (prototypeRef.type == CollectibleType::Score) {
        type = isReversed ? CollectibleType::Damage : CollectibleType::Score;
        usingFirstTexture = !isReversed;
        sprite.setTexture(isReversed ? prototypeRef.texture2 : prototypeRef.texture1);
    } else {
        type = isReversed ? CollectibleType::Score : CollectibleType::Damage;
        usingFirstTexture = isReversed;
        sprite.setTexture(isReversed ? prototypeRef.texture1 : prototypeRef.texture2);
    }

    entityData.type = (type == CollectibleType::Score) ? EntityType::COLLECTIBLE : EntityType::DAMAGE;

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    sprite.setScale({ 0.05f, 0.05f });
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Changed to dynamic so it can collide with kinematic bodies
    bodyDef.position.Set(startPosition.x / SCALE, startPosition.y / SCALE);
    bodyDef.linearVelocity.Set(initialVelocity.x / SCALE, initialVelocity.y / SCALE);
    bodyDef.gravityScale = 0.0f; // Disable gravity
    body = world->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = prototypeRef.radius / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    fixtureDef.density = 1.0f; // Required for dynamic bodies
    body->CreateFixture(&fixtureDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);

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
    position = sf::Vector2f(bodyPos.x * SCALE, bodyPos.y * SCALE);
    sprite.setPosition(position);

    // if (position.y > 1200.f) {
    //     isActive = false;
    // }
}

void Collectible::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);

        // debug draw collider wireframe
        // sf::CircleShape debugCircle(radius);
        // debugCircle.setOrigin({ radius, radius });
        // debugCircle.setPosition(position);
        // debugCircle.setFillColor(sf::Color::Transparent);
        // debugCircle.setOutlineColor(sf::Color::Blue);
        // debugCircle.setOutlineThickness(1.f);
        // window.draw(debugCircle);
    }
}

void Collectible::switchTexture() {
    if (!prototype) return;
    
    // Switch texture
    if (usingFirstTexture) {
        sprite.setTexture(prototype->texture2);
    }
    else {
        sprite.setTexture(prototype->texture1);
    }
    usingFirstTexture = !usingFirstTexture;
    
    // Switch type
    type = (type == CollectibleType::Score) ? CollectibleType::Damage : CollectibleType::Score;
    entityData.type = (type == CollectibleType::Score) ? EntityType::COLLECTIBLE : EntityType::DAMAGE;
    // body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
}

void Collectible::setPosition(sf::Vector2f pos) {
    position = pos;
    sprite.setPosition(position);
    if (body) {
        body->SetTransform(b2Vec2(position.x / SCALE, position.y / SCALE), 0.f);
    }
}
