#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Button.h"

class Menu {
public:
 
    Menu(float width, float height, bool exitMenu = false);

    int handleInput(const sf::RenderWindow& window);
    
    void draw(sf::RenderWindow& window);

private:

    sf::Texture m_bgTexture;  
    sf::Sprite m_bgSprite;        
    std::unique_ptr<Button> m_btn1; 
    std::unique_ptr<Button> m_btn2; 
    sf::Texture m_btn1Texture; 
    sf::Texture m_btn2Texture;
    bool exitMenu = false;
    
};