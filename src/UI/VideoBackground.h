#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class VideoBackground {
public:
    VideoBackground(
        const std::string& folder,
        const std::string& prefix,
        const std::string& extension,
        int imageCount,
        float fps
    );

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<sf::Texture> frames;
    std::vector<sf::Sprite> sprites;

    float frameTime;     // time per frame (1/fps)
    float timer = 0.f;
    size_t currentFrame = 0;
};
