#include <SFML/Graphics.hpp>
#include "InputHandler.h"

PlayerCommand InputHandler::getCommand()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        return PlayerCommand::moveLeft;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        return PlayerCommand::moveRight;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        return PlayerCommand::moveUp;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        return PlayerCommand::moveDown;

    return PlayerCommand::idle;
}