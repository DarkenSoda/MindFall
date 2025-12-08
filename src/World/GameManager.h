#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "../Player/Player.h"
#include "../World/EventHandler.h"
#include "../Time.h"

class GameManager
{
	InputHandler* inputHandler;
	Player* player;
	sf::View* gameView;
	b2World* world;
	EventHandler* eventHandler;
public:
	GameManager(InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world);
	~GameManager();
	void gameManagerUpdate();

};
