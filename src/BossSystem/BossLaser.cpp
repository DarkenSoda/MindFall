#include "BossLaser.h"
#include <iostream>

BossLaser::BossLaser(b2World* world, float maxWidth, float height, float windowWidth, float windowHeight)
    : world(world)
    , body(nullptr)
    , laserFixture(nullptr)
    , position(0.f, 0.f)
    , currentWidth(0.f)
    , maxWidth(maxWidth)
    , height(height)
    , state(LaserState::Inactive)
    , stateTimer(0.f)
    , expandDuration(0.3f)
    , activeDuration(2.0f)
    , shrinkDuration(0.3f)
    , windowWidth(windowWidth)
    , windowHeight(windowHeight)
    , entityData(EntityType::LASER, this) {
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(0.f, 0.f);
    
    body = world->CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
    
    laserShape.setOrigin({0.f, 0.f});

    loadTexture("assets/boss_laser.png");
}

BossLaser::~BossLaser() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

bool BossLaser::loadTexture(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load laser texture: " << texturePath << std::endl;
        return false;
    }
    
    laserShape.setTexture(&texture);
    return true;
}

void BossLaser::activate(sf::Vector2f bossPosition) {
    if (state != LaserState::Inactive) return;
    
    position = bossPosition;
    currentWidth = 0.f;
    state = LaserState::Expanding;
    stateTimer = 0.f;
    
    body->SetTransform(b2Vec2(position.x / SCALE, position.y / SCALE), 0.f);
}

void BossLaser::update(float deltaTime) {
    if (state == LaserState::Inactive) return;
    
    stateTimer += deltaTime;
    
    switch (state) {
        case LaserState::Expanding: {
            float progress = stateTimer / expandDuration;
            if (progress >= 1.0f) {
                currentWidth = maxWidth;
                state = LaserState::Active;
                stateTimer = 0.f;
            } else {
                currentWidth = maxWidth * progress;
            }
            updateCollider();
            break;
        }
        
        case LaserState::Active: {
            if (stateTimer >= activeDuration) {
                state = LaserState::Shrinking;
                stateTimer = 0.f;
            }
            break;
        }
        
        case LaserState::Shrinking: {
            float progress = stateTimer / shrinkDuration;
            if (progress >= 1.0f) {
                currentWidth = 0.f;
                state = LaserState::Inactive;
                if (laserFixture) {
                    body->DestroyFixture(laserFixture);
                    laserFixture = nullptr;
                }
            } else {
                currentWidth = maxWidth * (1.0f - progress);
            }
            updateCollider();
            break;
        }
        
        case LaserState::Inactive:
            break;
    }
    
    laserShape.setSize(sf::Vector2f(currentWidth, height));
    laserShape.setPosition({position.x - currentWidth / 2.f, 0.f});
    
    body->SetTransform(b2Vec2(position.x / SCALE, (height / 2.f) / SCALE), 0.f);
}

void BossLaser::updatePosition(sf::Vector2f bossPosition) {
    position = bossPosition;
}

void BossLaser::updateCollider() {
    if (laserFixture) {
        body->DestroyFixture(laserFixture);
        laserFixture = nullptr;
    }
    
    if (currentWidth > 0.f) {
        b2PolygonShape boxShape;
        boxShape.SetAsBox(
            (currentWidth / 2.f) / SCALE,
            (height / 2.f) / SCALE
        );
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.isSensor = true;
        fixtureDef.friction = 0.0f;
        
        laserFixture = body->CreateFixture(&fixtureDef);
        laserFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
    }
}

void BossLaser::render(sf::RenderWindow& window) {
    if (state == LaserState::Inactive || currentWidth <= 0.f) return;
    
    window.draw(laserShape);
    
    // // Debug visualization
    // if (currentWidth > 0.f) {
    //     sf::RectangleShape debugBox(sf::Vector2f(currentWidth, height));
    //     debugBox.setOrigin({0.f, 0.f});
    //     debugBox.setPosition({position.x - currentWidth / 2.f, 0.f});
    //     debugBox.setFillColor(sf::Color::Transparent);
    //     debugBox.setOutlineColor(sf::Color::Green);
    //     debugBox.setOutlineThickness(1.f);
    //     window.draw(debugBox);
    // }
}

void BossLaser::laserReset()
{
    state = LaserState::Inactive;
    currentWidth = 0.f;
    stateTimer = 0.f;
    if (laserFixture) {
        body->DestroyFixture(laserFixture);
        laserFixture = nullptr;
	}
}
