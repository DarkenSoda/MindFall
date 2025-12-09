#include "VideoBackground.h"
#include <iostream>

VideoBackground::VideoBackground(
    const std::string& folder,
    const std::string& prefix,
    const std::string& extension,
    int imageCount,
    float fps,
    bool loop
) {
    frameTime = 1.f / fps;


    frames.reserve(imageCount);
    sprites.reserve(imageCount);

    for (int i = 0; i < imageCount; i++) {
        std::string path = folder + "/" + prefix + std::to_string(i+1) + extension;

        // 2. Load into a temporary texture
        sf::Texture tempTexture;
        if (!tempTexture.loadFromFile(path)) {
            std::cerr << "Failed to load: " << path << "\n";
            continue; 
        }

        // 3. Move the texture into the vector
        // In SFML 3, Textures are move-only, so we must use std::move or emplace
        frames.push_back(std::move(tempTexture));


        sprites.emplace_back(frames.back()); 
        this->loop = loop;
    }
}

void VideoBackground::update(float dt) {
    if (!isPlaying) return;

    timer += dt;

    while (timer >= frameTime) {
        timer -= frameTime;
        // currentFrame = (currentFrame + 1) % sprites.size();
        if(loop)
            currentFrame = (currentFrame + 1) % sprites.size();
        else
        {
             if (currentFrame >= sprites.size() - 1 ) {
            stop(); // Animation finished: Hide and Reset
            return;
        }
        else
            currentFrame++;
        }
       
    }
}

void VideoBackground::play() {
    isPlaying = true;
}

void VideoBackground::stop() {
    isPlaying = false; 
    currentFrame = 0; 
    timer = 0.f;
}

void VideoBackground::draw(sf::RenderWindow& window) {
    if (!sprites.empty() && isPlaying)
        window.draw(sprites[currentFrame]);
}
