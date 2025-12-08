#include "Map.h"
#include "../EntityTypes.h"
#include <iostream>

static const float PPM = 30.0f;

// 1. UPDATED INIT (Only Ground)
void Map::init(b2World& world, const sf::Texture* groundTexture) {
    m_groundTexturePtr = groundTexture;

    // --- GROUND SETUP (Bottom 15% of 1080p) ---
    float w = 2000.f; 
    float h = 1080.f * 0.15f; 
    float x = 1920.f / 2.f; 
    float y = 1080.f - (h / 2.f); 

    // Setup Visuals
    m_groundShape.setSize({w, h});
    m_groundShape.setOrigin({w / 2.f, h / 2.f});
    m_groundShape.setPosition({x, y});

    if (m_groundTexturePtr != nullptr && m_groundTexturePtr->getSize().x > 0) {
        m_groundShape.setTexture(m_groundTexturePtr);
        m_groundShape.setTextureRect(sf::IntRect({0, 0}, {(int)w, (int)h})); // SFML 3 Syntax
        m_groundShape.setFillColor(sf::Color::White);
    } else {
        std::cerr << "Map Warning: Ground texture missing." << std::endl;
        m_groundShape.setFillColor(sf::Color(100, 250, 50)); 
    }

    // Setup Physics
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody; 
    bodyDef.position.Set(x / PPM, y / PPM); 

    m_groundBody = world.CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((w / 2.f) / PPM, (h / 2.f) / PPM); 

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.5f;
    fixtureDef.userData.pointer = static_cast<uintptr_t>(EntityType::GROUND);

    m_groundBody->CreateFixture(&fixtureDef);
}

// 2. NEW ADD LAYER FUNCTION
void Map::addLayer(const std::string& filePath, float speed) {
    auto newLayer = std::make_unique<ParallaxLayer>();
    
    // Calls the load function in ParallaxLayer
    if (newLayer->load(filePath, speed)) {
        m_parallaxLayers.push_back(std::move(newLayer));
    } else {
        std::cerr << "Map Error: Failed to add layer " << filePath << std::endl;
    }
}

// 3. NEW UPDATE FUNCTION
void Map::update(float dt) {
    for (auto& layer : m_parallaxLayers) {
        layer->update(dt);
    }
}

// 4. UPDATED DRAW
void Map::draw(sf::RenderWindow& window) {
    // Draw Backgrounds First
    for (auto& layer : m_parallaxLayers) {
        layer->draw(window);
    }

    // Draw Ground Last
    window.draw(m_groundShape);
}