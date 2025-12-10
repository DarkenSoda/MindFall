#include "SoundPlayer.h"
#include <iostream>

SoundPlayer::SoundPlayer() : buffer(), sound(buffer)
{
}

bool SoundPlayer::load(const std::string& path)
{
    if (!buffer.loadFromFile(path))
    {
        std::cout << "Failed to load sound: " << path << '\n';
        return false;
    }

    sound.setBuffer(buffer);
    return true;
}

void SoundPlayer::play()
{
    sound.play();
}
