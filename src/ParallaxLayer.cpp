#include "ParallaxLayer.h"
#include <iostream>

bool ParallaxLayer::load(const std::string& filePath, float scrollSpeed)
{
    if (!m_texture.loadFromFile(filePath)) {
        std::cerr << "Parallax Error: Failed to load " << filePath << std::endl;
        return false;
    }

    m_texture.setRepeated(true);

    // CHANGED: Create the sprites NOW using the loaded texture
    m_sprite1 = std::make_unique<sf::Sprite>(m_texture);
    m_sprite2 = std::make_unique<sf::Sprite>(m_texture);

    m_speed = scrollSpeed;

    // --- VERTICAL SETUP ---
    float height = static_cast<float>(m_texture.getSize().y);
    
    // Use '->' to access the sprite functions now
    m_sprite2->setPosition({0.0f, -height});

    return true;
}

void ParallaxLayer::update(float dt)
{
    // Safety check: ensure sprites exist before moving
    if (!m_sprite1 || !m_sprite2) return;

    // CHANGED: Use arrow syntax '->'
    m_sprite1->move({0.0f, m_speed * dt});
    m_sprite2->move({0.0f, m_speed * dt});

    float height = static_cast<float>(m_texture.getSize().y);

    // --- RESET LOGIC ---
    if (m_sprite1->getPosition().y >= height) {
        m_sprite1->setPosition({0.0f, m_sprite2->getPosition().y - height});
    }

    if (m_sprite2->getPosition().y >= height) {
        m_sprite2->setPosition({0.0f, m_sprite1->getPosition().y - height});
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window)
{
    // CHANGED: Dereference '*' the pointer to draw it
    if (m_sprite1) window.draw(*m_sprite1);
    if (m_sprite2) window.draw(*m_sprite2);
}