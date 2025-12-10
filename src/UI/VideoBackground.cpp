#include "VideoBackground.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

VideoBackground::VideoBackground(
    const std::string& folder,
    const std::string& prefix,
    const std::string& extension,
    int imageCount,
    float fps,
    bool loop
) {
    frameTime = 1.f / fps;
    this->loop = loop;

    frames.resize(imageCount);
    sprites.reserve(imageCount);

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    auto loadChunk = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            std::string path = folder + "/" + prefix + std::to_string(i + 1) + extension;

            if (!frames[i].loadFromFile(path)) {
                std::cerr << "Failed to load: " << path << "\n";
            }
        }
        };

    std::vector<std::thread> threads;
    int chunkSize = (imageCount + numThreads - 1) / numThreads;

    for (unsigned int t = 0; t < numThreads; t++) {
        int start = t * chunkSize;
        int end = std::min(start + chunkSize, imageCount);

        if (start < imageCount) {
            threads.emplace_back(loadChunk, start, end);
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (int i = 0; i < imageCount; i++) {
        if (frames[i].getSize().x > 0) {
            sprites.emplace_back(frames[i]);
        }
    }
}

void VideoBackground::update(float dt) {
    if (!isPlaying) return;

    timer += dt;

    while (timer >= frameTime) {
        timer -= frameTime;
        if (loop)
            currentFrame = (currentFrame + 1) % sprites.size();
        else {
            if (currentFrame >= sprites.size() - 1) {
                stop();
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
