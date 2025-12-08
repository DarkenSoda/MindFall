#ifndef PARALLAX_LAYER_H
#define PARALLAX_LAYER_H

#include <SFML/Graphics.hpp>

class ParallaxLayer {
public:
    ParallaxLayer();
    bool load(const std::string& file, float scrollSpeed);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite1;
    sf::Sprite sprite2;

    float speed;
};

#endif
