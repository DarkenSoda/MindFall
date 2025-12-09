#include "VideoBackground.hpp"
#include <iostream>

VideoBackground::VideoBackground(
    const std::string& folder,
    const std::string& prefix,
    const std::string& extension,
    int imageCount,
    float fps
) {
    frameTime = 1.f / fps;

    frames.resize(imageCount);
    sprites.resize(imageCount);

    for (int i = 0; i < imageCount; i++) {
        std::string path = folder + "/" + prefix + std::to_string(i) + extension;

        if (!frames[i].loadFromFile(path)) {
            std::cerr << "Failed to load: " << path << "\n";
            continue;
        }

        sprites[i].setTexture(frames[i]);
    }
}

void VideoBackground::update(float dt) {
    timer += dt;

    while (timer >= frameTime) {
        timer -= frameTime;
        currentFrame = (currentFrame + 1) % sprites.size();
    }
}

void VideoBackground::draw(sf::RenderWindow& window) {
    if (!sprites.empty())
        window.draw(sprites[currentFrame]);
}
