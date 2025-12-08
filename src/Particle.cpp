#include "Particle.h"

Particle::Particle(sf::Vector2f position, sf::Vector2f vel, sf::Color col, float life)
    : velocity(vel), lifetime(life), maxLifetime(life), color(col) {
    shape.setRadius(2.0f);
    shape.setFillColor(col);
    shape.setPosition(position);
    shape.setOrigin({ 1.0f, 1.0f });
}

void Particle::update(float dt) {
    lifetime -= dt;
    shape.move(velocity * dt);

    float alpha = (lifetime / maxLifetime) * 255.0f;
    color.a = static_cast<uint8_t>(alpha);
    shape.setFillColor(color);
}

bool Particle::isDead() const {
    return lifetime <= 0.0f;
}

void Particle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
