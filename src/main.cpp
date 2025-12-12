#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Time.h"
#include "World/GameManager.h"
#include "CollisionSystem/CollisionHandler.h"
#include "Player/InputHandler.h" 
#include "Player/Player.h"
#include "World/EventHandler.h"
#include "Player/Bullet.h"
#include "UI/RageBar.h"
#include <cmath>

const float WINDOW_WIDTH = 1920.f;
const float WINDOW_HEIGHT = 1080.f;

int main() 
{
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT) }), "MindFall", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    sf::Image icon;
    if (!icon.loadFromFile("assets/icon.ico")) {
        std::cout << "Failed to load icon image!" << std::endl;
    }
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    sf::Texture loadingTexture;
    
    if (loadingTexture.loadFromFile("assets/loading_screen.png")) {
        sf::Sprite loadingSprite(loadingTexture);
        window.clear(sf::Color::Black);
        window.draw(loadingSprite);
        window.display();
    } else {
        window.clear(sf::Color::Black);
        window.display();
    }

    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));

    b2World world(b2Vec2(0.f, 0.f));

    InputHandler inputHandler;
    Player player(&world, { WINDOW_WIDTH / 2.f, 800.f }, { 50.f, 90.f }, 300.f, WINDOW_WIDTH, WINDOW_HEIGHT);

    GameManager gameManager(&window, &inputHandler, &player, &gameView, &world);

    CollisionHandler collisionHandler(&gameManager);
    world.SetContactListener(&collisionHandler);

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