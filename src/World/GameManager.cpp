#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow* window, InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world)
	: startMenu(1920, 1080),
	gameOverMenu(1920, 1080, true),
	healthBar(nullptr),
	boss(world, { WINDOW_WIDTH / 2.f, -300.f }, { 450.f, 220.f }, WINDOW_WIDTH, WINDOW_HEIGHT),
	spawner(world),
	shootCooldown(sf::seconds(5.0f)),
	randomGen(std::random_device{}()),
	score(0),
	playTime(0.0f),
	bossState(BossState::NOT_SPAWNED),
	bossStartPosition(WINDOW_WIDTH / 2.f, -300.f),
	bossTargetPosition(WINDOW_WIDTH / 2.f, 162.f),
	bossIntroSpeed(200.0f),
	scoreText(uiFont, "", 30),
	playTimeText(uiFont, "", 30) {
	this->window = window;
	this->inputHandler = inputHandler;
	this->player = player;
	this->gameView = gameView;
	this->world = world;

	eventHandler = new EventHandler(inputHandler, player, gameView);
	videoBg = new VideoBackground("assets/VideoBackground", "", ".png", 63, 10.f);
	gameMap.init(*world);

	if (!healthBarTexture.loadFromFile("assets/player/hp.png")) {
		throw std::runtime_error("Failed to load health bar texture from assets/player/hp.png");
	}
	healthBar = new HealthBar(healthBarTexture, 5);

	if (!bulletTexture.loadFromFile("assets/player/bullet.png")) {
		throw std::runtime_error("Failed to load bullet texture from assets/player/bullet.png");
	}

	bulletTexture.setSmooth(true);
	if (!bulletTexture.generateMipmap()) {
		throw std::runtime_error("Failed to generate mipmaps for bullet texture");
	}

	if (!rageBarTexture.loadFromFile("assets/player/FINAL_LEVEL_UP.png")) {
		throw std::runtime_error("Failed to load bullet texture from assets/player/FINAL_LEVEL_UP.png");
	}

	rageBar = new RageBar(rageBarTexture, 5);

	if (!bulletUITexture.loadFromFile("assets/BulletUI.png")) {
		throw std::runtime_error("Failed to load bullet UI texture from assets/BulletUI.png");
	}

	if (!bulletUIGrayTexture.loadFromFile("assets/BulletUI_gray.png")) {
		throw std::runtime_error("Failed to load bullet UI gray texture from assets/BulletUI_gray.png");
	}

	bulletUITexture.setSmooth(true);
	bulletUIGrayTexture.setSmooth(true);

	if (!bulletUITexture.generateMipmap() || !bulletUIGrayTexture.generateMipmap()) {
		throw std::runtime_error("Failed to generate mipmaps for bullet UI texture");
	}

	bulletCooldownUI = new BulletCooldownUI(bulletUITexture, bulletUIGrayTexture);
	bulletCooldownUI->setPosition(sf::Vector2f(1450.0f, 965.0f));

	this->eventHandler->setSpawner(&spawner);

	// Load font for UI
	if (!uiFont.openFromFile("C:/Windows/Fonts/arial.ttf")) {
		throw std::runtime_error("Failed to load font for UI");
	}

	// Initialize score text
	scoreText.setFont(uiFont);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition({ 10.f, 10.f });

	// Initialize play time text
	playTimeText.setFont(uiFont);
	playTimeText.setCharacterSize(30);
	playTimeText.setFillColor(sf::Color::White);
	playTimeText.setPosition({ 10.f, 50.f });

	// Load bullet sound
	if (!bulletSound.load("assets/Sound/pew.mp3")) {
		std::cerr << "Warning: Failed to load pew sound" << std::endl;
	}

	if (!introMusic.load("assets/Sound/intro.mp3")) {
		std::cerr << "Warning: Failed to load intrp sound" << std::endl;
	}

	if (!bossMusic.load("assets/Sound/bossMusic.mp3")) {
		std::cerr << "Warning: Failed to load boss music sound" << std::endl;
	}

	if (!gameloopMusic.load("assets/Sound/gameloop.mp3")) {
		std::cerr << "Warning: Failed to load gameloop sound" << std::endl;
	}

	if (!ouchSound.load("assets/Sound/ouch.mp3")) {
		std::cerr << "Warning: Failed to load bullet sound" << std::endl;
	}

	if (!coinSound.load("assets/Sound/coin.mp3")) {
		std::cerr << "Warning: Failed to load coin sound" << std::endl;
	}

	if (!bossHurt.load("assets/Sound/bossHurt.mp3")) {
		std::cerr << "Warning: Failed to load coin sound" << std::endl;
	}

	introMusic.loop();
	bossMusic.loop();
	gameloopMusic.loop();
	introMusic.play();
}

GameManager::~GameManager()
{
	delete eventHandler;
	if (healthBar != nullptr) {
		delete healthBar;
	}
	delete rageBar;
	delete bulletCooldownUI;
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
				if (keyEvent->code == sf::Keyboard::Key::Space) {
					if (shootTimer.getElapsedTime() >= shootCooldown) {
						sf::Vector2f playerPos = player->getPlayerPosition();
						bullets.emplace_back(world, bulletTexture, playerPos.x + 45.f, playerPos.y, 20.f, 40.f);
						bulletSound.play();
						shootTimer.restart();
					}
				}
				
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
		playTime += Utils::Time::deltaTime;
		checkBossSpawn();

		if (bossState == BossState::INTRO) {
			updateBossIntro(Utils::Time::deltaTime);
			introMusic.Stop();
			bossMusic.play();
		} else if (bossState == BossState::ACTIVE) {
			boss.update(Utils::Time::deltaTime);
		}

		spawner.update(Utils::Time::deltaTime, WINDOW_WIDTH);

		world->Step(Utils::Time::fixedDeltaTime, 8, 3);

		eventHandler->handleEvent(Utils::Time::deltaTime);
		player->move(inputHandler->getCommand(), Utils::Time::deltaTime);
		player->update(Utils::Time::deltaTime);
		gameMap.update(Utils::Time::deltaTime);

		if (healthBar != nullptr) {
			int currentHealth = player->getLives();
			healthBar->setHealth(currentHealth);
		}
		rageBar->setRage(player->getRage());
		
		// Update bullet cooldown UI
		float currentCooldown = shootTimer.getElapsedTime().asSeconds();
		float maxCooldown = shootCooldown.asSeconds();
		bulletCooldownUI->setCooldown(maxCooldown - currentCooldown, maxCooldown);
		
		for (auto& bullet : bullets) {
			bullet.update();
		}

		bullets.erase(
			std::remove_if(bullets.begin(), bullets.end(),
				[](const Bullet& bullet) {
					auto bounds = bullet.getBounds();
					return bounds.position.y < -100.f || !bullet.isActive();
				}),
			bullets.end()
		);

		// Update particles
		for (auto& particle : particles) {
			particle.update(Utils::Time::deltaTime);
		}

		// Remove dead particles
		particles.erase(
			std::remove_if(particles.begin(), particles.end(),
				[](const Particle& p) { return p.isDead(); }),
			particles.end()
		);

		// Update score and play time text
        scoreText.setString("Score: " + std::to_string(score));
        playTimeText.setString("Time: " + std::to_string(static_cast<int>(playTime)) + "s");
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
        
        // Only render boss if it has spawned
        if (bossState != BossState::NOT_SPAWNED) {
            boss.render(*window);
        }
        
        player->drawPlayer(*window);

        // Draw bullets with game view
        for (auto& bullet : bullets) {
            bullet.render(*window);
        }

		// Draw particles with game view
		for (const auto& particle : particles) {
			const_cast<Particle&>(particle).draw(*window);
		}

        window->setView(window->getDefaultView());

        if (healthBar != nullptr) {
            healthBar->render(*window);
        }

        if (rageBar != nullptr) {
            rageBar->render(*window);
        }

        if (bulletCooldownUI != nullptr) {
            bulletCooldownUI->render(*window);
        }

		// Draw score and play time text
        window->draw(scoreText);
        window->draw(playTimeText);
    }
    else if (currentState == State::BEGINING_SCENE)
    {
        window->setView(window->getDefaultView());
        videoBg->draw(*window);
    }
	else if (currentState == State::GAME_OVER)
	{
		//resets
		inputHandler->setDirection(false);
		gameView->setRotation(sf::degrees(0.0f));
		bullets.clear();
		particles.clear();
		player->resetPlayer();
		spawner.resetSpawner();
		boss.resetBoss();

		delete eventHandler;
		eventHandler = new EventHandler(inputHandler, player, gameView);
		eventHandler->setSpawner(&spawner);
	
		score = 0;
		playTime = 0.0f;
		bossState = BossState::NOT_SPAWNED;

		gameOverMenu.draw(*window);
	}
}

void GameManager::applyDamageToPlayer()
{
	ouchSound.play();
	player->takeDamage(1);
	
	if (player->getLives() <= 0) {
		currentState = State::GAME_OVER;
	}
}

void GameManager::applyDamageToBoss()
{
	bossHurt.play();
	boss.takeDamage();
	
	if (boss.getHP() <= 0) {
		currentState = State::GAME_OVER;
	}
}

void GameManager::spawnParticleAt(sf::Vector2f position, sf::Color color, int count)
{
	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
	std::uniform_real_distribution<float> speedDist(50.0f, 200.0f);
	std::uniform_real_distribution<float> lifeDist(0.5f, 1.5f);

	for (int i = 0; i < count; ++i) {
		float angle = angleDist(randomGen);
		float speed = speedDist(randomGen);
		sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

		// Randomize color slightly for variety
		sf::Color particleColor = color;
		particleColor.r = std::min(255, static_cast<int>(color.r * (0.8f + 0.4f * static_cast<float>(rand()) / RAND_MAX)));
		particleColor.g = std::min(255, static_cast<int>(color.g * (0.8f + 0.4f * static_cast<float>(rand()) / RAND_MAX)));
		particleColor.b = std::min(255, static_cast<int>(color.b * (0.8f + 0.4f * static_cast<float>(rand()) / RAND_MAX)));

		particles.emplace_back(position, velocity, particleColor, lifeDist(randomGen));
	}
}

void GameManager::addScore(int amount)
{
	coinSound.play();
	score += amount;
}

void GameManager::checkBossSpawn()
{
	if (bossState == BossState::NOT_SPAWNED) {
		if (score >= SCORE_TO_SPAWN_BOSS || playTime >= TIME_TO_SPAWN_BOSS) {
			bossState = BossState::INTRO;
			boss.setIntroPhase(true);
			
			// Reset boss position to start position
			b2Body* bossBody = boss.getBody();
			if (bossBody) {
				bossBody->SetTransform(
					b2Vec2(bossStartPosition.x / 30.f, bossStartPosition.y / 30.f),
					0.0f
				);
				
				for (b2Fixture* fixture = bossBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
					fixture->SetSensor(true);
				}
			}
		}
	}
}

void GameManager::updateBossIntro(float deltaTime)
{
	b2Body* bossBody = boss.getBody();
	if (!bossBody) return;

	sf::Vector2f currentPos = boss.getPosition();
	
	if (currentPos.y < bossTargetPosition.y) {
		// Set downward velocity
		bossBody->SetLinearVelocity(b2Vec2(0.f, bossIntroSpeed / 30.f));
		
		boss.update(Utils::Time::deltaTime);
		
		if (boss.getPosition().y >= bossTargetPosition.y) {
			bossState = BossState::ACTIVE;
			boss.setIntroPhase(false);
			
			bossBody->SetLinearVelocity(b2Vec2(0.f, 0.f));
			
			bossBody->SetTransform(
				b2Vec2(bossTargetPosition.x / 30.f, bossTargetPosition.y / 30.f),
				0.0f
			);
			
			for (b2Fixture* fixture = bossBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
				fixture->SetSensor(false);
			}
		}
	} else {
		bossState = BossState::ACTIVE;
		boss.setIntroPhase(false);
		bossBody->SetLinearVelocity(b2Vec2(0.f, 0.f));
		
		for (b2Fixture* fixture = bossBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			fixture->SetSensor(false);
		}
	}
}