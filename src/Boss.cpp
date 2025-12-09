#include "Boss.h"
#include <cmath>
#include <iostream>

Boss::Boss(b2World* world, sf::Vector2f startPosition, sf::Vector2f bossSize)
    : world(world)
    , position(startPosition)
    , size(bossSize)
    , body(nullptr)
    , pupilLayerIndex(-1)
    , pupilCircleRadius(10.f)
    , pupilAngle(0.f)
    , pupilOffset(0.f, 0.f)
    , playerPosition(0.f, 0.f)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(startPosition.x, startPosition.y);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(bossSize.x / 2.f, bossSize.y / 2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    // load all textures

}

Boss::~Boss() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void Boss::addLayer(const std::string& texturePath, sf::Vector2f offset, bool isAnimated) {
    BossLayer layer;
    
    if (!layer.texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load boss layer texture: " << texturePath << std::endl;
        return;
    }

    layer.sprite.setTexture(layer.texture);
    layer.offset = offset;
    layer.isAnimated = isAnimated;

    sf::FloatRect bounds = layer.sprite.getLocalBounds();
    layer.sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    layers.push_back(std::move(layer));
}

void Boss::setPupilLayer(int layerIndex, float circleRadius) {
    if (layerIndex >= 0 && layerIndex < static_cast<int>(layers.size())) {
        pupilLayerIndex = layerIndex;
        pupilCircleRadius = circleRadius;
        layers[layerIndex].isAnimated = true;
    }
}

void Boss::setPlayerPosition(sf::Vector2f playerPos) {
    playerPosition = playerPos;
}

void Boss::update(float deltaTime) {
    b2Vec2 bodyPos = body->GetPosition();
    position = sf::Vector2f(bodyPos.x, bodyPos.y);

    if (pupilLayerIndex >= 0 && pupilLayerIndex < static_cast<int>(layers.size())) {
        sf::Vector2f directionToPlayer = playerPosition - position;
        float distance = std::sqrt(directionToPlayer.x * directionToPlayer.x + 
                                   directionToPlayer.y * directionToPlayer.y);

        if (distance > 0.f) {
            directionToPlayer.x /= distance;
            directionToPlayer.y /= distance;

            pupilOffset = directionToPlayer * pupilCircleRadius;
        } else {
            pupilOffset = sf::Vector2f(0.f, 0.f);
        }
    }

    for (size_t i = 0; i < layers.size(); ++i) {
        sf::Vector2f layerPosition = position + layers[i].offset;

        if (static_cast<int>(i) == pupilLayerIndex) {
            layerPosition += pupilOffset;
        }

        layers[i].sprite.setPosition(layerPosition);
    }
}

void Boss::render(sf::RenderWindow& window) {
    for (const auto& layer : layers) {
        window.draw(layer.sprite);
    }

    sf::RectangleShape debugBox(sf::Vector2f(size.x, size.y));
    debugBox.setOrigin({ size.x / 2.f, size.y / 2.f });
    debugBox.setPosition(position);
    debugBox.setFillColor(sf::Color::Transparent);
    debugBox.setOutlineColor(sf::Color(0, 255, 0, 255));
    debugBox.setOutlineThickness(2.f);
    window.draw(debugBox);
}
