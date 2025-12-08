#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerCommand.h"

class InputHandler {
public:
    PlayerCommand getCommand();
};
