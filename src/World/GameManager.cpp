#include "GameManager.h"
#include <iostream>

GameManager::GameManager(sf::RenderWindow* window, InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world)
	: startMenu(1920, 1080), 
	  gameOverMenu(1920, 1080, true) // Pass true for exitMenu variant
{
	this->window = window;
	this->inputHandler = inputHandler;
	this->player = player;
	this->gameView = gameView;
	this->world = world;
	
	eventHandler = new EventHandler(inputHandler, player, gameView);
	videoBg = new VideoBackground("assets/VideoBackground", "", ".png", 63, 10.f);
	// Initialize the map with the world passed to GM
	gameMap.init(*world);
}

GameManager::~GameManager()
{
	delete eventHandler;
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

			// Game specific keys
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

			gameView->setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
		}
	}
}

void GameManager::gameManagerUpdate()
{
	// 1. Handle Menu Logic
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

		world->Step(Utils::Time::fixedDeltaTime, 8, 3);

		eventHandler->handleEvent();
		

		player->move(inputHandler->getCommand(), Utils::Time::deltaTime);
		player->update(Utils::Time::deltaTime);
		gameMap.update(Utils::Time::deltaTime);
	}
	if(currentState == State::BEGINING_SCENE)
	{
		
		videoBg->update(Utils::Time::deltaTime);
		cout << videoBg->isPlaying << "\n";
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
	 if (currentState == State::PLAYING)
	{
		//cout <<"Play\n";
		window->setView(*gameView);
		
		gameMap.draw(*window);
		// player->drawPlayer(*window);
	}
	//  if (currentState == State::GAME_OVER)
	// {
	// 	window->setView(window->getDefaultView());
	// 	gameOverMenu.draw(*window);
	// }
	 if (currentState == State::BEGINING_SCENE)
	{
		cout << "Drawing video background\n";
		window->setView(window->getDefaultView());
		videoBg->draw(*window);
	}
}