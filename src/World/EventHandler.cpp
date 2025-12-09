#include "EventHandler.h"
#include <iostream>
using namespace std;

EventHandler::EventHandler() {}

EventHandler::EventHandler(InputHandler* inputHandler, Player* player, sf::View* gameView)
{
    this->inputHandler = inputHandler;
    this->player = player;
    this->gameView = gameView;
}

EventHandler::~EventHandler() {}

void EventHandler::handleEvent(float dt)
{
    if (player->getRage() >= 100.0f && !eventDone)
    {
        eventDone = true;

        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(1, 3);
        int r = distribution(generator);

        cout << "Random number: " << r << endl;

        switch (r)
        {
        case 1:
            flipDirectionEvent();
            break;
        case 2:
            flipCameraEvent(180.0f);
            flipDirectionEvent();
            break;
        case 3:
            flipCollectableColors();
            break;
        }
    }

    if (player->getRage() <= 0.0f && eventDone)
    {
        eventDone = false;
        flipCameraEvent(0.0f);
        inputHandler->setDirection(false);
        if (spawner) {
            spawner->resetType();

        }
    }

    if (rotating)
    {
        rotationProgress += dt / rotationTime;
        if (rotationProgress >= 1.0f)
        {
            rotationProgress = 1.0f;
            rotating = false;
        }

        float newAngle = startRot + (targetRot - startRot) * rotationProgress;
        gameView->setRotation(sf::degrees(newAngle));
    }
}

void EventHandler::flipDirectionEvent()
{
    inputHandler->setDirection(true);
}

void EventHandler::flipCameraEvent(float angle)
{
    if (!rotating)
    {
        startRot = gameView->getRotation().asDegrees();
        targetRot = angle;
        rotationProgress = 0.0f;
        rotating = true;
    }
}

void EventHandler::flipCollectableColors()
{
    if (spawner) {
        spawner->switchAllCollectible();
    }
}

void EventHandler::setSpawner(WorldSpawner* spawner) {
    this->spawner = spawner;
}
