#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <memory> 
#include <string> 

#include "../ParallaxLayer.h" 
#include "../UI/VideoBackground.h"
#include "../EntityTypes.h"
#include "../EntityData.h"

class Map {
public:
    void init(b2World& world);
    void addLayer(const std::string& filePath, float speed);
    void update(float dt);
    void playVideo();

    void draw(sf::RenderWindow& window);

private:
    const float SCALE = 30.0f;

    std::vector<std::unique_ptr<ParallaxLayer>> m_parallaxLayers;
    std::unique_ptr<VideoBackground> m_videoBackground;
    std::unique_ptr<VideoBackground> m_ambientBackground;
    sf::RectangleShape m_groundShape;
    b2Body* m_groundBody = nullptr;
    sf::Texture m_groundTexture;
    EntityData m_groundEntityData { EntityType::GROUND, nullptr };
    
    // Collider debug draw data
    sf::Vector2f m_colliderSize;
    sf::Vector2f m_colliderPosition;
};