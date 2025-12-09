#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include "../Player/Player.h"
#include "../Player/Bullet.h"
#include "../World/EventHandler.h"
#include "../Time.h"
#include "../UI/Menu.h"
#include "../UI/HealthBar.h"
#include "../Environment/Map.h"
#include "../Spawner/WorldSpawner.h"
#include "../BossSystem/Boss.h"

class GameManager
{
public:
	enum class State {
		MAIN_MENU,
		PLAYING,
		GAME_OVER,
		BEGINING_SCENE
	};

private:
	sf::RenderWindow* window;
	InputHandler* inputHandler;
	Player* player;
	sf::View* gameView;
	b2World* world;
	EventHandler* eventHandler;
	
	Menu startMenu;
	Menu gameOverMenu;
	HealthBar* healthBar;	
	sf::Texture healthBarTexture;
	
	// Bullet management
	std::vector<Bullet> bullets;
	sf::Texture bulletTexture;
	sf::Clock shootTimer;
	sf::Time shootCooldown;
	
	State currentState = State::MAIN_MENU;
	Map gameMap;
	VideoBackground* videoBg;

	Boss boss;
	WorldSpawner spawner;

	// Constants for view resizing
	const float WINDOW_WIDTH = 1920.f;
	const float WINDOW_HEIGHT = 1080.f;

public:
	GameManager(sf::RenderWindow* window, InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world);
	~GameManager();

	// Handles window events (Close, Resize, Key presses)
	void handleEvents();
	
	// Updates game logic (Physics, Menus, Entities)
	void gameManagerUpdate();
	
	// Renders the current state
	void gameManagerRender();
};