#include "GameManager.h"


GameManager::GameManager(InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world)
{
	this->inputHandler = inputHandler;
	this->player = player;
	this->gameView = gameView;
	this->world = world;
	eventHandler = new EventHandler(inputHandler, player, gameView);
}

GameManager::~GameManager()
{
	delete eventHandler;
}

void GameManager::gameManagerUpdate()
{
	eventHandler->handleEvent();
	player->move(inputHandler->getCommand(), Utils::Time::deltaTime);
	player->update(Utils::Time::deltaTime);
}
