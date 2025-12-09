#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Time.h"
#include "World/GameManager.h"
#include "CollisionSystem/CollisionHandler.h"
#include "Player/InputHandler.h" 
#include "Player/Player.h"
#include "World/EventHandler.h"
#include "Player/Bullet.h"

// Constants
const float WINDOW_WIDTH = 1920.f;
const float WINDOW_HEIGHT = 1080.f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT) }), "MiniJam", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    // Setup Dependencies
    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
    
    b2World world(b2Vec2(0.f, 0.f));
    CollisionHandler collisionHandler;
    world.SetContactListener(&collisionHandler);

    
    InputHandler inputHandler; 
    Player player({ 200.f, 800.f }, 200.f);

    // Create Game Manager
    GameManager gameManager(&window, &inputHandler, &player, &gameView, &world);


    sf::Texture bulletTex;
    if (!bulletTex.loadFromFile("assets/player/bullet.png")) {
        std::cout << "Error loading bullet.png" << std::endl;
        // In a real game, handle error. For now, we continue or exit.
        return -1;
    }

    std::vector<Bullet> bullets; // This stores all active bullets
    sf::Clock shootTimer;
    sf::Time shootCooldown = sf::seconds(1.25f);

    while (window.isOpen()) {
        
        // Update Delta Time
        Utils::Time::Update();

        // 1. Handle Events (Inputs, Resize, Window Close)
        gameManager.handleEvents();

        gameManager.gameManagerUpdate();

        // 3. Render
        window.clear();
        for (auto& bullet : bullets) {
            bullet.render(window);
        }
        player.drawPlayer(window);
        window.setView(gameView);
        window.display();
    }

    return 0;
}