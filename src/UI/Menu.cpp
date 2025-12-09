#include "Menu.h"

Menu::Menu(float width, float height, bool exitMenu): m_bgSprite(m_bgTexture), exitMenu(exitMenu) 
{
    
    if (!m_bgTexture.loadFromFile("assets/images.jpeg")) {
        throw std::runtime_error("Failed to load background texture");
    }
    m_bgSprite.setTexture(m_bgTexture);
    sf::Vector2u texSize = m_bgTexture.getSize();
    
    m_bgSprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(texSize.x), static_cast<int>(texSize.y)}));
    if (texSize.x > 0 && texSize.y > 0) {
        float scaleX = width / static_cast<float>(texSize.x);
        float scaleY = height / static_cast<float>(texSize.y);
        m_bgSprite.setScale({scaleX, scaleY});
    }

    
    if(!exitMenu)
        m_btn1Texture.loadFromFile("assets/start.jpg");

    else
        m_btn1Texture.loadFromFile("assets/tryAgain.jpeg");

    if (!m_btn2Texture.loadFromFile("assets/exit.png")) {
        throw std::runtime_error("Failed to load button 2 texture");
    }


    float centerX1 = (width / 2.0f) - (m_btn1Texture.getSize().x / 2.0f);
    float centerX2 = (width / 2.0f) - (m_btn2Texture.getSize().x / 2.0f);

    m_btn1 = std::make_unique<Button>(centerX1, 200.f, m_btn1Texture);
    m_btn2 = std::make_unique<Button>(centerX2, 550.f, m_btn2Texture);
}

int Menu::handleInput(const sf::RenderWindow& window) {

    static bool isHandled = false; 

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!isHandled) {

            if (m_btn1 && m_btn1->isClicked(window)) { 
                isHandled = true; 
                return 1; 
            }

            if (m_btn2 && m_btn2->isClicked(window)) { 
                isHandled = true; 
                return 2; 
            }
            
            isHandled = true;
        }
    } else {
        isHandled = false; 
    }
    
    return 0; 
}

void Menu::draw(sf::RenderWindow& window) {

    window.draw(m_bgSprite);
    
    if (m_btn1) m_btn1->draw(window);
    if (m_btn2) m_btn2->draw(window);
}