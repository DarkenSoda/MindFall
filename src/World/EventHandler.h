#pragma once
#include <iostream>
#include "../Player/InputHandler.h"
#include "../Player/Player.h"
#include <SFML/Graphics.hpp>
#include <random>

class EventHandler {
private:
    InputHandler* inputHandler;
    Player* player;
    sf::View* gameView;
    bool eventDone = false;

    bool rotating = false;
    float startRot = 0.0f;
    float targetRot = 0.0f;
    float rotationTime = 0.5f;
    float rotationProgress = 0.0f;

public:
    EventHandler();
    EventHandler(InputHandler* inputHandler, Player* player, sf::View* gameView);
    ~EventHandler();

    void handleEvent(float dt);

    void flipDirectionEvent();
    void flipCameraEvent(float angle);
    void flipCollectableColors();
};
