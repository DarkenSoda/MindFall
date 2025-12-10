#include "Player.h"
#include <iostream>
using namespace std;

void Player::animations()
{
    animator
        .SetAnimation("idle", std::make_unique<Animation>("assets/player/happy.png", 0.125f, 1))
        .SetAnimation("stress", std::make_unique<Animation>("assets/player/stressed.png", 0.125f, 1));
}

Player::Player(b2World* world, sf::Vector2f position, sf::Vector2f size, float moveSpeed, float windowWidth, float windowHeight)
    : world(world)
    , position(position)
    , size(size)
    , body(nullptr)
    , entityData(EntityType::PLAYER, this)
    , windowWidth(windowWidth)
    , windowHeight(windowHeight)
{
    this->moveSpeed = moveSpeed;
    animations();
    
	startPosition = position;
    rage = 1.0f;
    multiplier = 8.0f;
    rageDirection = false;
    maximumRage = 100.0f;
    minimumRage = 0.0f;
    timeToEmptyRage = 5.0f;
    rageDownCooldown = 0.0f;
    playerParts = new PlayerParts(position);
    lives = 5;
	state = "idle";
    
    isInvulnerable = false;
    invulnerabilityTimer = 0.0f;
    invulnerabilityDuration = 1.0f;
    flashTimer = 0.0f;
    flashInterval = 0.1f;
    isFlashVisible = true;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.gravityScale = 0.0f;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    
    body = world->CreateBody(&bodyDef);
    
    auto spriteBounds = animator.CurrentAnimaton().getGlobalBounds();
    float offsetX = (spriteBounds.size.x / 2.f) / SCALE;
    float offsetY = (spriteBounds.size.y / 2.f) / SCALE;
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox((size.x / 2.f) / SCALE, (size.y / 2.f) / SCALE, b2Vec2(offsetX, offsetY), 0.f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.isSensor = true;
    fixtureDef.friction = 0.0f;
    
    body->CreateFixture(&fixtureDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);
}

Player::~Player()
{
    delete playerParts;
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void Player::move(PlayerCommand cmd, float deltaTime)
{
    float velocityX = 0.f;
    
    switch (cmd)
    {
    case PlayerCommand::moveLeft:
        velocityX = -moveSpeed / SCALE;
        break;

    case PlayerCommand::moveRight:
        velocityX = moveSpeed / SCALE;
        break;

    default:
        velocityX = 0.f;
        break;
    }
    
    body->SetLinearVelocity(b2Vec2(velocityX, 0.f));
    
    b2Vec2 bodyPos = body->GetPosition();
    position = sf::Vector2f(bodyPos.x * SCALE, bodyPos.y * SCALE);
    
    auto spriteBounds = animator.CurrentAnimaton().getGlobalBounds();
    float offsetX = spriteBounds.size.x / 2.f;
    
    float halfWidth = size.x / 2.f;
    float leftBound = -offsetX + halfWidth;
    float rightBound = windowWidth - offsetX - halfWidth;
    
    if (position.x < leftBound) {
        position.x = leftBound;
        body->SetTransform(b2Vec2(position.x / SCALE, bodyPos.y), 0.f);
        body->SetLinearVelocity(b2Vec2(0.f, 0.f));
    }
    else if (position.x > rightBound) {
        position.x = rightBound;
        body->SetTransform(b2Vec2(position.x / SCALE, bodyPos.y), 0.f);
        body->SetLinearVelocity(b2Vec2(0.f, 0.f));
    }
    
    playerParts->updateParts(position);
    animator.PlayAnimation(state, deltaTime, position);
    
    if (isInvulnerable) {
        invulnerabilityTimer += deltaTime;
        flashTimer += deltaTime;
        
        if (flashTimer >= flashInterval) {
            isFlashVisible = !isFlashVisible;
            flashTimer = 0.0f;
        }
        
        if (invulnerabilityTimer >= invulnerabilityDuration) {
            isInvulnerable = false;
            invulnerabilityTimer = 0.0f;
            isFlashVisible = true;
        }
    }
}

void Player::setPlayerPosition(sf::Vector2f position)
{
    this->position = position;
}

sf::Vector2f Player::getPlayerPosition()
{
    return position;
}

float Player::getRage()
{
    return rage;
}

float Player::getRageInterval() const {
    // Map rage to the nearest multiple of 20
    return std::clamp(static_cast<int>(rage / 20) * 20, 0, 100);
}

int Player::getLives() const
{
    return lives;
}

void Player::takeDamage(int damage)
{
    if (isInvulnerable) {
        return;
    }
    
    lives -= damage;
    if (lives < 0)
        lives = 0;
    
    isInvulnerable = true;
    invulnerabilityTimer = 0.0f;
    flashTimer = 0.0f;
    isFlashVisible = true;
}

sf::FloatRect Player::getGlobalBound()
{
    return animator.CurrentAnimaton().getGlobalBounds();
}

void Player::drawPlayer(sf::RenderWindow& window)
{
    sf::Color colorEffect;
    if (isInvulnerable && isFlashVisible) {
        colorEffect = sf::Color(255, 100, 100);
    } else if (isInvulnerable && !isFlashVisible) {
        colorEffect = sf::Color(255, 255, 255, 255);
    } else {
        colorEffect = sf::Color::White;
    }
    
    playerParts->setColor(colorEffect);
    playerParts->drawBehindParts(window);
    
    sf::Sprite& sprite = animator.CurrentAnimaton();
    sprite.setColor(colorEffect);
    
    window.draw(sprite);
    playerParts->drawFrontParts(window);

    // Debug draw collider
    // b2Vec2 bodyPos = body->GetPosition();
    // auto spriteBounds = animator.CurrentAnimaton().getGlobalBounds();
    // float offsetX = spriteBounds.size.x / 2.f;
    // float offsetY = spriteBounds.size.y / 2.f;
    
    // sf::RectangleShape debugBox(sf::Vector2f(size.x, size.y));
    // debugBox.setPosition(sf::Vector2f(
    //     bodyPos.x * SCALE + offsetX - size.x / 2.f, 
    //     bodyPos.y * SCALE + offsetY - size.y / 2.f
    // ));
    // debugBox.setFillColor(sf::Color::Transparent);
    // debugBox.setOutlineColor(sf::Color::Green);
    // debugBox.setOutlineThickness(2.f);
    // window.draw(debugBox);
}

void Player::update(float deltatime)
{
    if (rage >= maximumRage)
    {
        rageDirection = true;
        rage = 100.0f;
        state = "stress";
        playerParts->setRotationSpeed(20.0f);
        moveSpeed = 400.0f;
    }
    if (rage <= minimumRage)
    {
        rageDirection = false;
        multiplier = 8.0f;
        rage = 0.0f;
        state = "idle";
        playerParts->setRotationSpeed(10.0f);
        moveSpeed = 300.0f;
    }
    if (!rageDirection)
    {
        rage += deltatime * multiplier;
    }
    else
    {
        float interval = 100.0f / timeToEmptyRage;
        rageDownCooldown += deltatime;
        if (rageDownCooldown >= 1.0f)
        {
            rage -= interval;
            rageDownCooldown = 0.0f;
        }
    }
}

void Player::resetPlayer()
{
    lives = 5;
    rage = 0.0f;
    rageDirection = false;
    state = "idle";
    playerParts->setRotationSpeed(10.0f);
	moveSpeed = 300.0f;
    position = startPosition;
	body->SetTransform(b2Vec2(position.x / SCALE, position.y / SCALE), 0.f);
    isInvulnerable = false;
    invulnerabilityTimer = 0.0f;
    isFlashVisible = true;
    
}