#include "Boss.h"
#include <cmath>
#include <iostream>
#include <random>

Boss::Boss(b2World* world, sf::Vector2f startPosition, sf::Vector2f bossSize, float windowWidth, float windowHeight)
    : world(world)
    , position(startPosition)
    , size(bossSize)
    , body(nullptr)
    , currentAnimation("")
    , currentFrame(0)
    , animationTimer(0.f)
    , moveSpeed(400.f)
    , movementDirection(1.0f)
    , leftBound(0.f)
    , rightBound(1920.f)
    , attackTimer(0.f)
    , attackInterval(3.0f)
    , windowWidth(windowWidth)
    , windowHeight(windowHeight)
    , laser(world, 110.f, 1080.f, windowWidth, windowHeight)
    , sprite(idleTexture)
    , hp(3)
    , entityData(EntityType::BOSS, this)
    , isIntroPhase(true)
    , startPosition(position)
{
    laser.setExpandDuration(0.4f);
    laser.setActiveDuration(1.0f);
    laser.setShrinkDuration(0.3f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(startPosition.x / SCALE, startPosition.y / SCALE);

    body = world->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((bossSize.x / 2.f) / SCALE, (bossSize.y / 2.f) / SCALE, b2Vec2(0.f, -50.f / SCALE), 0.f);

    b2FixtureDef fixtureDef;
    fixtureDef.friction = 0.0f;
    fixtureDef.shape = &boxShape;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entityData);

    sf::Texture projectileTex;
    if (!projectileTex.loadFromFile("assets/boss_projectile.png")) {
        std::cerr << "Failed to load boss projectile texture." << std::endl;
    }
    projectileTex.setSmooth(true);
    if (!projectileTex.generateMipmap()) {
        std::cerr << "Failed to generate mipmaps for boss projectile texture." << std::endl;
    }

    attackTextures.push_back(projectileTex);

    loadSprites("assets/boss1.png", "assets/boss2.png", "assets/boss3.png");

    playAnimation("idle");
}

Boss::~Boss() {
    if (body && world) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

bool Boss::loadSprites(const std::string& idlePath, const std::string& laserPath, const std::string& projectilePath) {
    if (!idleTexture.loadFromFile(idlePath)) {
        std::cerr << "Failed to load boss idle texture: " << idlePath << std::endl;
        return false;
    }
    if (!laserTexture.loadFromFile(laserPath)) {
        std::cerr << "Failed to load boss laser texture: " << laserPath << std::endl;
        return false;
    }
    if (!projectileTexture.loadFromFile(projectilePath)) {
        std::cerr << "Failed to load boss projectile texture: " << projectilePath << std::endl;
        return false;
    }

    addAnimation("idle", idleTexture, 5, 0.15f, true);
    addAnimation("laser", laserTexture, 9, 0.2f, false);
    addAnimation("projectile", projectileTexture, 5, 0.2f, false);

    return true;
}

void Boss::addAnimation(const std::string& name, sf::Texture& texture, int frameCount, float switchTime, bool loop) {
    int frameWidth = texture.getSize().x / frameCount;
    int frameHeight = texture.getSize().y;
    
    animations[name] = BossAnimation(&texture, frameCount, frameWidth, frameHeight, switchTime, loop);
}

void Boss::playAnimation(const std::string& name) {
    if (currentAnimation == name) return;
    
    if (animations.find(name) != animations.end()) {
        currentAnimation = name;
        currentFrame = 0;
        animationTimer = 0.f;
        
        const BossAnimation& anim = animations[name];
        sprite.setTexture(*anim.texture);
        sprite.setTextureRect(sf::IntRect({0, 0}, {anim.frameWidth, anim.frameHeight}));
        sprite.setOrigin({ anim.frameWidth / 2.f, anim.frameHeight / 2.f });
        sprite.setScale({0.2f, 0.2f});
    }
}

void Boss::setBounds(float left, float right) {
    leftBound = left;
    rightBound = right;
}

void Boss::setMoveSpeed(float speed) {
    moveSpeed = speed;
}

void Boss::setAttackInterval(float interval) {
    attackInterval = interval;
}

void Boss::update(float deltaTime) {
    b2Vec2 bodyPos = body->GetPosition();
    sf::Vector2f newPosition = sf::Vector2f(bodyPos.x * SCALE, bodyPos.y * SCALE);

    position = newPosition;

    // Skip movement and attacks during intro phase
    if (!isIntroPhase) {
        float buffer = 5.f;
        if (position.x <= leftBound + size.x / 2.f + buffer && movementDirection < 0.f) {
            movementDirection = 1.0f;
        }
        else if (position.x >= rightBound - size.x / 2.f - buffer && movementDirection > 0.f) {
            movementDirection = -1.0f;
        }

        body->SetLinearVelocity(b2Vec2((moveSpeed / SCALE) * movementDirection, 0.f));

        updateAttacks(deltaTime);
    } else {
        body->SetLinearVelocity(b2Vec2(0.f, body->GetLinearVelocity().y));
    }

    updateAnimation(deltaTime);
    
    laser.updatePosition(position);
    laser.update(deltaTime);
    
    for (auto& projectile : projectiles) {
        if (projectile && projectile->isActive()) {
            projectile->update(deltaTime);
        }
    }
    
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::unique_ptr<BossProjectile>& p) { return !p || !p->isActive(); }),
        projectiles.end());
    
    sprite.setPosition(position);
}

void Boss::updateAnimation(float deltaTime) {
    if (currentAnimation.empty() || animations.find(currentAnimation) == animations.end()) {
        return;
    }

    const BossAnimation& anim = animations[currentAnimation];
    animationTimer += deltaTime;

    if (animationTimer >= anim.switchTime) {
        currentFrame++;
        
        if (currentFrame >= anim.frameCount) {
            if (anim.loop) {
                currentFrame = 0;
            } else {
                currentFrame = anim.frameCount - 1;
                playAnimation("idle");
            }
        }

        sprite.setTextureRect(sf::IntRect(
            {currentFrame * anim.frameWidth, 0},
            {anim.frameWidth, anim.frameHeight}
        ));
        
        animationTimer = 0.f;
    }
}

void Boss::updateAttacks(float deltaTime) {
    attackTimer += deltaTime;
    
    if (attackTimer >= attackInterval) {
        performAttack();
        attackTimer = 0.f;
    }
}

void Boss::performAttack() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 100);
    
    int choice = dis(gen);
    
    if (choice <= 70) {
        projectileAttack();
        playAnimation("projectile");
    }
    else {
        laserAttack();
        playAnimation("laser");
    }
}

void Boss::projectileAttack() {
    sf::Vector2f spawnPos = position;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> countDis(1, 2);
    static std::uniform_int_distribution<> sideDis(0, 1);
    
    int bulletCount = countDis(gen);
    float speed = 500.f;
    
    for (int i = 0; i < bulletCount; ++i) {
        float angle;
        if (sideDis(gen) == 0) {
            static std::uniform_real_distribution<float> rightAngleDis(30.f, 60.f);
            angle = rightAngleDis(gen);
        } else {
            static std::uniform_real_distribution<float> leftAngleDis(120.f, 150.f);
            angle = leftAngleDis(gen);
        }
        
        float angleRad = angle * 3.14159f / 180.f;
        
        sf::Vector2f velocity(
            std::cos(angleRad) * speed,
            std::sin(angleRad) * speed
        );
        
        auto projectile = std::make_unique<BossProjectile>(
            world, spawnPos, velocity, 28.f, windowWidth, windowHeight
        );

        projectile->setTexture(attackTextures[0]);

        projectiles.push_back(std::move(projectile));
    }
}

void Boss::laserAttack() {
    if (!laser.isActive()) {
        laser.activate(position);
    }
}

void Boss::render(sf::RenderWindow& window) {
    laser.render(window);
    
    for (auto& projectile : projectiles) {
        if (projectile && projectile->isActive()) {
            projectile->render(window);
        }
    }
    
    window.draw(sprite);

    // sf::RectangleShape debugBox(sf::Vector2f(size.x, size.y));
    // debugBox.setOrigin({ size.x / 2.f, size.y / 2.f });
    // debugBox.setPosition(sf::Vector2f(position.x, position.y - 50.f));
    // debugBox.setFillColor(sf::Color::Transparent);
    // debugBox.setOutlineColor(sf::Color(0, 255, 0, 255));
    // debugBox.setOutlineThickness(2.f);
    // window.draw(debugBox);
}

void Boss::takeDamage() {
    if (hp > 0) {
        hp--;
    }
}

void Boss::resetBoss()
{
	position = startPosition;
	body->SetTransform(b2Vec2(startPosition.x / SCALE, startPosition.y / SCALE), 0.f);
    playAnimation("idle");
	attackTimer = 0.f;
	hp = 3;
	isIntroPhase = true;

	projectiles.clear();
	laser.laserReset();
}
