#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>

class SoundPlayer
{
private:
    sf::SoundBuffer buffer;
    sf::Sound sound;

public:
    SoundPlayer();

    bool load(const std::string& path);
    void play();
};
