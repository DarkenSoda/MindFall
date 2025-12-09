#include <iostream>
#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow* window, InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world)
	: startMenu(1920, 1080), 
	gameOverMenu(1920, 1080, true),
	healthBar(nullptr),
	boss(world, { WINDOW_WIDTH / 2.f, 162.f }, { 450.f, 220.f }, WINDOW_WIDTH, WINDOW_HEIGHT),
	spawner(world)
{
	this->window = window;
	this->inputHandler = inputHandler;
	this->player = player;
	this->gameView = gameView;
	this->world = world;
	
	eventHandler = new EventHandler(inputHandler, player, gameView);
	videoBg = new VideoBackground("assets/VideoBackground", "", ".png", 63, 10.f);
	gameMap.init(*world);

	if (healthBarTexture.loadFromFile("assets/player/hb.png")) {
		healthBar = new HealthBar(healthBarTexture, 6);
	}

	this->eventHandler->setSpawner(&spawner);
}

GameManager::~GameManager()
{
	delete eventHandler;
	if (healthBar != nullptr) {
		delete healthBar;
	}
}

void GameManager::handleEvents()
{
	while (const std::optional event = window->pollEvent()) 
	{
		if (event->is<sf::Event::Closed>())
			window->close();

		if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) 
		{
			if (keyEvent->code == sf::Keyboard::Key::Escape)
				window->close();

			if (currentState == State::PLAYING) {
				if (keyEvent->code == sf::Keyboard::Key::K) {
					gameMap.playVideo();
				}
				if (keyEvent->code == sf::Keyboard::Key::P) {
					currentState = State::GAME_OVER;
				}
			}
		}

		if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()) 
		{
			float windowRatio = static_cast<float>(resizeEvent->size.x) / static_cast<float>(resizeEvent->size.y);
			float viewRatio = WINDOW_WIDTH / WINDOW_HEIGHT;

			float sizeX = 1.f;
			float sizeY = 1.f;
			float posX = 0.f;
			float posY = 0.f;

			if (windowRatio > viewRatio) {
				sizeX = viewRatio / windowRatio;
				posX = (1.f - sizeX) / 2.f;
			}
			else {
				sizeY = windowRatio / viewRatio;
				posY = (1.f - sizeY) / 2.f;
			}

			gameView->setViewport(sf::FloatRect(sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY)));
		}
	}
}

void GameManager::gameManagerUpdate()
{
	if (currentState == State::MAIN_MENU) 
	{
		int action = startMenu.handleInput(*window);
		if (action == 1) {
			currentState = State::BEGINING_SCENE;
			videoBg->play();
		}
		else if (action == 2) {
			window->close();
		}
	}
	if (currentState == State::GAME_OVER) 
	{
		int action = gameOverMenu.handleInput(*window);
		if (action == 1) {
			currentState = State::PLAYING;
		}
		else if (action == 2) {
			window->close();
		}
	}

	if (currentState == State::PLAYING)
	{
		boss.update(Utils::Time::deltaTime);
		spawner.update(Utils::Time::deltaTime, WINDOW_WIDTH);

		world->Step(Utils::Time::fixedDeltaTime, 8, 3);

		eventHandler->handleEvent(Utils::Time::deltaTime);
		player->move(inputHandler->getCommand(), Utils::Time::deltaTime);
		player->update(Utils::Time::deltaTime);
		gameMap.update(Utils::Time::deltaTime);

		// Update HealthBar based on player's current health
		if (healthBar != nullptr) {
			int currentHealth = player->getLives();
			healthBar->setHealth(currentHealth);
		}
	}
	if(currentState == State::BEGINING_SCENE)
	{
		videoBg->update(Utils::Time::deltaTime);
		if(!(videoBg->isPlaying))
		{
			currentState = State::PLAYING;
			gameMap.playVideo();
		}
	}
}

void GameManager::gameManagerRender()
{
	if (currentState == State::MAIN_MENU)
	{
		window->setView(window->getDefaultView());
		startMenu.draw(*window);
	}
	else if (currentState == State::PLAYING)
	{
		// Draw game world with game view
		window->setView(*gameView);
		gameMap.draw(*window);
		spawner.draw(*window);
		boss.render(*window);
		player->drawPlayer(*window);

		// Switch to default view for UI layer
		window->setView(window->getDefaultView());
		
		// Draw HealthBar on top
		if (healthBar != nullptr)
		{
			healthBar->render(*window);
		}
	}
	else if (currentState == State::BEGINING_SCENE)
	{
		window->setView(window->getDefaultView());
		videoBg->draw(*window);
	}
}