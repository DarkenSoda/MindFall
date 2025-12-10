#include "Bullet.h"

Bullet::Bullet(b2World* world, const sf::Texture& texture, float startX, float startY, float bulletWidth, float bulletHeight)
    : world(world)
    , sprite(texture)
    , position(startX, startY)
    , size(bulletWidth, bulletHeight)
    , body(nullptr)
    , active(true)
    , entityData(EntityType::BULLET, this)
{
    speed = 500.f;

    sprite.setScale({ 0.05f, 0.05f });
    sprite.setOrigin(sprite.getLocalBounds().size / 2.f);
    sprite.setPosition({ startX, startY });
    
    // Create Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.gravityScale = 0.0f;
    bodyDef.position.Set(startX / SCALE, startY / SCALE);

    body = world->CreateBody(&bodyDef);
    
    // Create Box2D fixture (collision shape)
    b2PolygonShape boxShape;
    boxShape.SetAsBox((bulletWidth / 2.f) / SCALE, (bulletHeight / 2.f) / SCALE);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.isSensor = true;
    fixtureDef.friction = 0.0f;
    
    body->CreateFixture(&fixtureDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
    
    // Set upward velocity
    body->SetLinearVelocity(b2Vec2(0.f, -speed / SCALE));
}

Bullet::Bullet(Bullet&& other) noexcept
    : world(other.world)
    , sprite(std::move(other.sprite))
    , position(other.position)
    , size(other.size)
    , body(other.body)
    , active(other.active)
    , speed(other.speed)
    , entityData(EntityType::BULLET, this) {
    other.body = nullptr;
    other.world = nullptr;

    // Update user data pointer to point to this object
    if (body) {
        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
    }
}

Bullet& Bullet::operator=(Bullet&& other) noexcept {
    if (this != &other) {
        // Clean up existing body
        if (body && world) {
            world->DestroyBody(body);
        }

        // Move data from other
        world = other.world;
        sprite = std::move(other.sprite);
        position = other.position;
        size = other.size;
        body = other.body;
        active = other.active;
        speed = other.speed;

        // Clear other
        other.body = nullptr;
        other.world = nullptr;

        // Update user data pointer to point to this object
        if (body) {
            body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
        }
    }
    return *this;
}

Bullet::~Bullet() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void Bullet::update() {
    if (!active) return;
    
    // Update position from Box2D
    b2Vec2 bodyPos = body->GetPosition();
    position = sf::Vector2f(bodyPos.x * SCALE, bodyPos.y * SCALE);
    
    sprite.setPosition(position);
}

void Bullet::render(sf::RenderWindow& window) {
    if (!active) return;
    
    window.draw(sprite);
    
    // Debug draw collision box
    // b2Vec2 bodyPos = body->GetPosition();
    // sf::RectangleShape debugBox(sf::Vector2f(size.x, size.y));
    // debugBox.setPosition(sf::Vector2f(bodyPos.x * SCALE - size.x / 2.f, bodyPos.y * SCALE - size.y / 2.f));
    // debugBox.setFillColor(sf::Color::Transparent);
    // debugBox.setOutlineColor(sf::Color::Cyan);
    // debugBox.setOutlineThickness(2.f);
    // window.draw(debugBox);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}