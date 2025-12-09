#pragma once
#include <SFML/Graphics.hpp>

struct BossAnimation {
    sf::Texture* texture;
    int frameCount;
    int frameWidth;
    int frameHeight;
    float switchTime;
    bool loop;

    BossAnimation(sf::Texture* tex = nullptr, int fc = 1, int fw = 0, int fh = 0, float st = 0.1f, bool l = true)
        : texture(tex), frameCount(fc), frameWidth(fw), frameHeight(fh), switchTime(st), loop(l) {
    }
};