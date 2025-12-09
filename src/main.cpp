#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Time.h"
#include "CollisionSystem/CollisionHandler.h"
#include "Spawner/WorldSpawner.h"
#include <cmath>
#include "BossSystem/Boss.h"

const float WINDOW_WIDTH = 1920.f;
const float WINDOW_HEIGHT = 1080.f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT) }), "MiniJam", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));

    b2World world(b2Vec2(0.f, 0.f));
    CollisionHandler collisionHandler;
    world.SetContactListener(&collisionHandler);

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

        world.Step(Utils::Time::fixedDeltaTime, 8, 3);

        window.clear();
        window.setView(gameView);

        window.display();
    }
}
