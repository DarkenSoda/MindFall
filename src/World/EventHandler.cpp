#include "EventHandler.h"
#include <iostream>
using namespace std;

EventHandler::EventHandler()
{
};

EventHandler::EventHandler(InputHandler* inputHandler, Player* player, sf::View* gameView) {
	this->inputHandler = inputHandler;
	this->player = player ;
	this->gameView = gameView;
}

EventHandler::~EventHandler()
{
};

void EventHandler::handleEvent() {
	if (player->getRage() >= 100.0f&& !eventDone)
	{
		eventDone = true;

		std::mt19937 generator(std::random_device{}());
		std::uniform_int_distribution<int> distribution(1, 3);
		int r = distribution(generator);

		std::cout << "Random number: " << r << std::endl;
		switch (r)
		{
		case 1:
			flipDirectionEvent();
			break;
		case 2:
			flipCameraEvent();
			flipDirectionEvent();
			break;
		case 3:
			flipCollectableColors();
			break;
		}
	}
	if (player->getRage() <= 0.0f)
	{
		eventDone = false;
		gameView->setRotation(sf::degrees(0.0f));
		inputHandler->setDirection(false);
	}
}

void EventHandler::flipDirectionEvent()
{
	inputHandler->setDirection(true);
}

void EventHandler::flipCameraEvent()
{
	gameView->rotate(sf::radians(3.14159f));
}

void EventHandler::flipCollectableColors()
{

}


