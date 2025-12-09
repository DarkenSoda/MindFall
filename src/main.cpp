#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Time.h"
#include "World/GameManager.h"
#include "CollisionSystem/CollisionHandler.h"
#include "Player/InputHandler.h" 
#include "Player/Player.h"
#include "World/EventHandler.h"
#include "Player/Bullet.h"
#include <cmath>

// Constants
const float WINDOW_WIDTH = 1920.f;
const float WINDOW_HEIGHT = 1080.f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT) }), "MiniJam"/*, sf::State::Fullscreen*/);
    window.setFramerateLimit(60);

    // Setup Dependencies
    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
    
    b2World world(b2Vec2(0.f, 0.f));
    CollisionHandler collisionHandler;
    world.SetContactListener(&collisionHandler);

    InputHandler inputHandler; 
    Player player({ 200.f, 800.f }, 200.f);

    GameManager gameManager(&window, &inputHandler, &player, &gameView, &world);

    while (window.isOpen()) {
        Utils::Time::Update();

        gameManager.handleEvents();

        window.clear();
        window.setView(gameView);
        gameManager.gameManagerUpdate();
        gameManager.gameManagerRender();
        window.display();
    }

    return 0;
}