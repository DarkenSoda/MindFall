#pragma once

struct BossAnimation {
    int row;
    int frameCount;
    float switchTime;
    bool loop;

    BossAnimation(int r = 0, int fc = 1, float st = 0.1f, bool l = true)
        : row(r), frameCount(fc), switchTime(st), loop(l) {
    }
};