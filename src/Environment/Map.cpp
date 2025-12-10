#include "Map.h"
#include "../EntityTypes.h"
#include <iostream>

void Map::init(b2World& world) {
    if (!m_groundTexture.loadFromFile("assets/FINAL_FLOOR.png")) {
        std::cerr << "Map Error: Failed to load assets/FINAL_FLOOR.png" << std::endl;
        m_groundShape.setFillColor(sf::Color(100, 250, 50));
    } 

    

    // --- GROUND SETUP (Bottom 15% of 1080p) ---
    float w = 2000.f; 
    float h = 1080.f * 0.23f; 
    float x = 1920.f / 2.f; 
    float y = 1080.f - (h / 2.f); 
    
    // Collider dimensions (scaled down vertically)
    float colliderHeight = h * 0.6f; // 60% of visual height
    float colliderY = y + (h / 2.f) - (colliderHeight / 2.f); // Position at bottom of visual

    m_videoBackground = std::make_unique<VideoBackground>("assets/Background_frames", "b", ".png", 9, 10.f, true);

    // Setup Visuals
    m_groundShape.setSize({w, h});
    m_groundShape.setOrigin({w / 2.f, h / 2.f});
    m_groundShape.setPosition({x, y});

    if (m_groundTexture.getSize().x > 0 ) {
        m_groundShape.setTexture(&m_groundTexture);
        m_groundShape.setTextureRect(sf::IntRect({0, 0}, {(int)w, (int)h})); // SFML 3 Syntax
        m_groundShape.setFillColor(sf::Color::White);
    } else {
        std::cerr << "Map Warning: Ground texture missing." << std::endl;
        m_groundShape.setFillColor(sf::Color(100, 250, 50)); 
    }

    // Setup Physics
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody; 
    bodyDef.position.Set(x / SCALE, colliderY / SCALE); 

    m_groundBody = world.CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((w / 2.f) / SCALE, (colliderHeight / 2.f) / SCALE); 

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.5f;

    m_groundBody->CreateFixture(&fixtureDef);
    m_groundBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(&m_groundEntityData);
    
    // Store collider data for debug draw
    m_colliderSize = {w, colliderHeight};
    m_colliderPosition = {x, colliderY};
}


void Map::addLayer(const std::string& filePath, float speed) {
    auto newLayer = std::make_unique<ParallaxLayer>();
    

    if (newLayer->load(filePath, speed)) {
        m_parallaxLayers.push_back(std::move(newLayer));
    } else {
        std::cerr << "Map Error: Failed to add layer " << filePath << std::endl;
    }
}

void Map::playVideo() {
    if (m_videoBackground) {
        m_videoBackground->play();
    }
}



void Map::update(float dt) {
    for (auto& layer : m_parallaxLayers) {
        layer->update(dt);
    }
    if (m_videoBackground) {
        m_videoBackground->update(dt);
    }
}


void Map::draw(sf::RenderWindow& window) {
    if (m_videoBackground) {
        m_videoBackground->draw(window);
    }
    for (auto& layer : m_parallaxLayers) {
        layer->draw(window);
    }

    window.draw(m_groundShape);

    // debug draw ground collider wireframe
    // sf::RectangleShape debugRect;
    // debugRect.setSize(m_colliderSize);
    // debugRect.setOrigin({m_colliderSize.x / 2.f, m_colliderSize.y / 2.f});
    // debugRect.setPosition(m_colliderPosition);
    // debugRect.setFillColor(sf::Color::Transparent);
    // debugRect.setOutlineColor(sf::Color::Red);
    // debugRect.setOutlineThickness(2.f);
    // window.draw(debugRect);
}