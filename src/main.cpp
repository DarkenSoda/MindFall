#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Time.h"
#include "CollisionSystem/CollisionHandler.h"
#include <cmath>
#include "UI/menu.h"
#include "Environment/Map.h"

enum class State {
    MAIN_MENU,
    PLAYING,
    GAME_OVER
};

const float WINDOW_WIDTH = 1920.f;
const float WINDOW_HEIGHT = 1080.f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT) }), "MiniJam", sf::State::Fullscreen);
    window.setFramerateLimit(60);


    sf::Texture bgMain, bgGameOver;
    sf::Texture btnStart, btnQuit, btnRetry;

    if (!bgMain.loadFromFile("../../../Assets/images.jpeg")) return -1;
    if (!bgGameOver.loadFromFile("../../../Assets/images.jpeg")) return -1;

    if (!btnStart.loadFromFile("../../../Assets/start.jpg")) return -1;
    if (!btnQuit.loadFromFile("../../../Assets/exit.png")) return -1;
    if (!btnRetry.loadFromFile("../../../Assets/tryAgain.jpeg")) return -1;


    Menu mainMenu(WINDOW_WIDTH, WINDOW_HEIGHT, bgMain, btnStart, btnQuit);
    Menu gameOverMenu(WINDOW_WIDTH, WINDOW_HEIGHT, bgGameOver, btnRetry, btnQuit);
    State currentState = State::MAIN_MENU;


    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));

    b2World world(b2Vec2(0.f, 0.f));
    CollisionHandler collisionHandler;
    world.SetContactListener(&collisionHandler);


      Map gameMap;
    
    gameMap.init(world, nullptr);
    gameMap.addLayer("../../../Assets/background_2.png", 20.f);
    gameMap.addLayer("../../../Assets/background_3.png", 40.f);
    gameMap.addLayer("../../../Assets/background_4.png", 80.f);


    while (window.isOpen()) {
        Utils::Time::Update();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if(const auto& keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                    window.close();
            }


            if (event->is<sf::Event::Resized>()) {
                const auto& resizeEvent = event->getIf<sf::Event::Resized>();

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

                gameView.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
            }
        }

        if(currentState == State::MAIN_MENU){
            int action = mainMenu.handleInput(window);
            if(action == 1){
                currentState = State::PLAYING;
            } else if(action == 2){
                window.close();
            }
        }
        else if (currentState == State::PLAYING){
            world.Step(Utils::Time::fixedDeltaTime, 8, 3);
        }
        else if (currentState == State::GAME_OVER){
            int action = gameOverMenu.handleInput(window);
            if(action == 1){
                currentState = State::PLAYING;
            } else if(action == 2){
                window.close();
            }
        }
        



        window.clear();
        if(currentState == State::MAIN_MENU){
            mainMenu.draw(window);
        }
        else if (currentState == State::PLAYING){
            gameMap.update(Utils::Time::fixedDeltaTime);
            gameMap.draw(window);
        }
        else if (currentState == State::GAME_OVER){
            gameOverMenu.draw(window);
        }
        

        window.display();
    }
}
