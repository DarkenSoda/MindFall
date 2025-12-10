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
#include "../UI/RageBar.h"
#include "../UI/BulletCooldownUI.h"
#include "../Particle.h"

class GameManager {
public:
	enum class State {
		MAIN_MENU,
		PLAYING,
		GAME_OVER,
		BEGINING_SCENE
	};

private:
	// Constants for view resizing
	const float WINDOW_WIDTH = 1920.f;
	const float WINDOW_HEIGHT = 1080.f;

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

    RageBar* rageBar;
    sf::Texture rageBarTexture;

	// Bullet cooldown UI
	BulletCooldownUI* bulletCooldownUI;
	sf::Texture bulletUITexture;
	sf::Texture bulletUIGrayTexture;

	std::vector<Particle> particles;
	
	std::mt19937 randomGen;

	int score;
	float playTime;
	const int SCORE_TO_SPAWN_BOSS = 10;
	const float TIME_TO_SPAWN_BOSS = 120.0f; // 2 minutes

	enum class BossState {
		NOT_SPAWNED,
		INTRO,
		ACTIVE
	};
	BossState bossState;
	sf::Vector2f bossStartPosition;
	sf::Vector2f bossTargetPosition;
	float bossIntroSpeed;

	void checkBossSpawn();
	void updateBossIntro(float deltaTime);

public:
	GameManager(sf::RenderWindow* window, InputHandler* inputHandler, Player* player, sf::View* gameView, b2World* world);
	~GameManager();

	// Handles window events (Close, Resize, Key presses)
	void handleEvents();

	// Updates game logic (Physics, Menus, Entities)
	void gameManagerUpdate();

	// Renders the current state
	void gameManagerRender();

	// Game state modification functions
	void applyDamageToPlayer();
	void applyDamageToBoss();
	void spawnParticleAt(sf::Vector2f position, sf::Color color, int count = 20);
	void addScore(int amount = 1);

	// Getters
	int getScore() const { return score; }
	float getPlayTime() const { return playTime; }
	bool isBossActive() const { return bossState == BossState::ACTIVE; }
};