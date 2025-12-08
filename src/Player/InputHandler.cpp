#include <SFML/Graphics.hpp>
#include "InputHandler.h"
#include <iostream>

PlayerCommand InputHandler::getCommand()
{
    if (!direction)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            return PlayerCommand::moveLeft;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            return PlayerCommand::moveRight;
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            return PlayerCommand::moveRight;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            return PlayerCommand::moveLeft;
    }
    return PlayerCommand::idle;
}
