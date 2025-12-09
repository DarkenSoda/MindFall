#pragma once
#include <iostream>
#include "../Player/InputHandler.h"
#include "../Player/Player.h"
#include <SFML/Graphics.hpp>
#include <random>

class EventHandler {
private:
	InputHandler* inputHandler;
	Player* player;
	sf::View* gameView;
	bool eventDone = false;
public:
	EventHandler();
	EventHandler(InputHandler* inputHandler , Player* player , sf::View* gameView);
	~EventHandler();
	void handleEvent();
	void flipDirectionEvent();
	void flipCameraEvent();
	void flipCollectableColors();
};
