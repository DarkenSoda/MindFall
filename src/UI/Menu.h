#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Button.h"

class Menu {
public:
 
    Menu(float width, float height, 
         const sf::Texture& bgTexture, 
         const sf::Texture& btn1Texture, 
         const sf::Texture& btn2Texture);

    int handleInput(const sf::RenderWindow& window);
    
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite m_bgSprite;          
    std::unique_ptr<Button> m_btn1; 
    std::unique_ptr<Button> m_btn2; 
};