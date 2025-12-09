#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory> 

class ParallaxLayer {
public:
    ParallaxLayer() = default;

    bool load(const std::string& filePath, float scrollSpeed);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture m_texture;
    
    // CHANGED: Use unique_ptr so they can start empty (nullptr)
    std::unique_ptr<sf::Sprite> m_sprite1;
    std::unique_ptr<sf::Sprite> m_sprite2;
    
    float m_speed = 0.0f;
};