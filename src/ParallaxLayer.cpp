#include "ParallaxLayer.h"
#include <iostream>

bool ParallaxLayer::load(const std::string& filePath, float scrollSpeed)
{
    if (!m_texture.loadFromFile(filePath)) {
        std::cerr << "Parallax Error: Failed to load " << filePath << std::endl;
        return false;
    }

    m_texture.setRepeated(true);


    m_sprite1 = std::make_unique<sf::Sprite>(m_texture);
    m_sprite2 = std::make_unique<sf::Sprite>(m_texture);

    m_speed = scrollSpeed;


    float height = static_cast<float>(m_texture.getSize().y);

    m_sprite2->setPosition({0.0f, -height});

    return true;
}

void ParallaxLayer::update(float dt)
{

    if (!m_sprite1 || !m_sprite2) return;


    m_sprite1->move({0.0f, m_speed * dt});
    m_sprite2->move({0.0f, m_speed * dt});

    float height = static_cast<float>(m_texture.getSize().y);


    if (m_sprite1->getPosition().y >= height) {
        m_sprite1->setPosition({0.0f, m_sprite2->getPosition().y - height});
    }

    if (m_sprite2->getPosition().y >= height) {
        m_sprite2->setPosition({0.0f, m_sprite1->getPosition().y - height});
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window)
{

    if (m_sprite1) window.draw(*m_sprite1);
    if (m_sprite2) window.draw(*m_sprite2);
}