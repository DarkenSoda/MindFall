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

void SoundPlayer::Stop()
{
	sound.stop();
}

void SoundPlayer::loop()
{
	sound.setLooping(true);
}

void SoundPlayer::setVolume(float volume)
{
    sound.setVolume(volume);
}

void SoundPlayer::setPitch(float pitch)
{
    sound.setPitch(pitch);
}