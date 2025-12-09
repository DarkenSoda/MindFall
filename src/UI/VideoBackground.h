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
        float fps,
        bool loop = false
    );

    void play();
    void stop();

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<sf::Texture> frames;
    std::vector<sf::Sprite> sprites;

    float frameTime;   
    float timer = 0.f;
    size_t currentFrame = 0;
    bool isPlaying = false;
    bool loop;
};
