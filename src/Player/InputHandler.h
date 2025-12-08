#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerCommand.h"

class InputHandler {
    bool direction = false;
public:
    PlayerCommand getCommand();
	void setDirection(bool dir) { direction = dir; }
	bool getDirection() const { return direction; }
};
