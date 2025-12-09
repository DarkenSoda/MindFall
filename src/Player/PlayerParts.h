#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

class PlayerParts
{
	string path[5];
	Texture texture[5];
	vector<Sprite> sprite;
	int length;
	float rotationSpeed = 10.0f;
	float rotationAmount = 10.0f;
	float xOffset = 40.0f;
	float yOffset = 80.0f;
public:
	PlayerParts(sf::Vector2f position);
	void updateParts(sf::Vector2f position);
	void drawBehindParts(RenderWindow& window);
	void drawFrontParts(RenderWindow& window);
	void rotateWheels();
	void setRotationSpeed(float speed);
};

