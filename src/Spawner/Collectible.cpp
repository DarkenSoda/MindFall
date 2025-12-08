#include "Collectible.h"

Collectible::Collectible(const std::string& texture1Path, const std::string& texture2Path,
    CollectibleType collectibleType, float spawnPercent, float colliderRadius)
    : type(collectibleType)
    , spawnPercentage(spawnPercent)
    , radius(colliderRadius)
    , body(nullptr)
    , world(nullptr)
    , position(0.f, 0.f)
    , isActive(false)
    , usingFirstTexture(true)
    , sprite(texture1) {

    if (!texture1Path.empty() && !texture1.loadFromFile(texture1Path)) {
        std::cerr << "Failed to load texture: " << texture1Path << std::endl;
    }
    if (!texture2Path.empty() && !texture2.loadFromFile(texture2Path)) {
        std::cerr << "Failed to load texture: " << texture2Path << std::endl;
    }

    sprite.setTexture(texture1);
    sprite.setOrigin({ 0.0f, 0.0f });
    sprite.setScale({ 0.1f, 0.1f });

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

Collectible::~Collectible() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

std::unique_ptr<Collectible> Collectible::clone(b2World* world, sf::Vector2f startPosition, sf::Vector2f initialVelocity, CollectibleType type) const {
    auto cloned = std::unique_ptr<Collectible>(new Collectible("", "", type, spawnPercentage, radius));

    cloned->texture1 = this->texture1;
    cloned->texture2 = this->texture2;

    if (type == CollectibleType::Score) {
        cloned->sprite.setTexture(cloned->texture1);
        cloned->usingFirstTexture = true;
    }
    else {
        cloned->sprite.setTexture(cloned->texture2);
        cloned->usingFirstTexture = false;
    }

    sf::FloatRect bounds = cloned->sprite.getLocalBounds();
    cloned->sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    cloned->world = world;
    cloned->position = startPosition;
    cloned->isActive = true;

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(startPosition.x, startPosition.y);
    bodyDef.linearVelocity.Set(initialVelocity.x, initialVelocity.y);
    cloned->body = world->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    cloned->body->CreateFixture(&fixtureDef);
    cloned->body->GetUserData().pointer = reinterpret_cast<uintptr_t>(cloned.get());

    cloned->sprite.setPosition(startPosition);

    return cloned;
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

        // debug draw collider
        sf::CircleShape debugCircle(radius);
        debugCircle.setOrigin({radius, radius});
        debugCircle.setPosition(position);
        debugCircle.setFillColor(sf::Color(255, 0, 0, 100));
        window.draw(debugCircle);
    }
}

void Collectible::switchTexture() {
    if (usingFirstTexture) {
        sprite.setTexture(texture2);
    }
    else {
        sprite.setTexture(texture1);
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
