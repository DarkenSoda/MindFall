#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <memory> 
#include <string> // Required for std::string

// Check your file structure! 
// If ParallaxLayer.h is in 'src/', keep "../ParallaxLayer.h"
// If ParallaxLayer.h is in 'src/Environment/', change to "ParallaxLayer.h"
#include "../ParallaxLayer.h" 
#include "../UI/VideoBackground.h"

class Map {
public:

    void init(b2World& world, const sf::Texture* groundTexture);

    void addLayer(const std::string& filePath, float speed);

    void update(float dt);

    void playVideo();

    void draw(sf::RenderWindow& window);

private:
    const sf::Texture* m_groundTexturePtr = nullptr;


    std::vector<std::unique_ptr<ParallaxLayer>> m_parallaxLayers;
    std::unique_ptr<VideoBackground> m_videoBackground; 
    sf::RectangleShape m_groundShape;
    b2Body* m_groundBody = nullptr;
};