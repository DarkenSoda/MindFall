#pragma once
#include <SFML/Graphics.hpp>

struct Particle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;
    sf::Color color;

    Particle(sf::Vector2f position, sf::Vector2f vel, sf::Color col, float life = 1.0f);
    
    void update(float dt);
    bool isDead() const;
    void draw(sf::RenderWindow& window);
};
