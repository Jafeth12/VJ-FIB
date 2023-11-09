#include "Goomba.h"
#include "Constants.h"
#include "TileMap.h"

#define ANIM_SPEED 8
#define GOOMBA_SIZE_IN_SPRITESHEET 32.f
#define GOOMBA_SIZE glm::ivec2(32, 32)
#define GOOMBA_SPEED 120.f

Texture *Goomba::s_goombaTexture = nullptr;

void Goomba::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &pos) {
    dir = initialDirection;
    tileMapDispl = tileMapPos;
    map = tileMap;
    enemySize = GOOMBA_SIZE;

    if (s_goombaTexture == nullptr) {
        s_goombaTexture = new Texture();
        s_goombaTexture->loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
        s_goombaTexture->setMinFilter(GL_NEAREST);
        s_goombaTexture->setMagFilter(GL_NEAREST);
    }
    spritesheet = s_goombaTexture;

    float size = GOOMBA_SIZE_IN_SPRITESHEET / spritesheet->width();
    sprite = Sprite::createSprite(enemySize, glm::vec2(size, size), s_goombaTexture, &shaderProgram);

    float row;
    if (color == Color::OVERWORLD)
        row = 0.f;
    else // UNDERWORLD
        row = 1.f;

    // Only 2 animations
    sprite->setNumberAnimations(2);

    enum_t walkId = getAnimId(State::WALK);
    enum_t deadId = getAnimId(State::DEAD);

    // WALK
    sprite->setAnimationSpeed(walkId, ANIM_SPEED);
    sprite->addKeyframe(walkId, glm::vec2(0.f, row));
    sprite->addKeyframe(walkId, glm::vec2(1.f, row));

    // DEAD
    sprite->setAnimationSpeed(deadId, ANIM_SPEED);
    sprite->addKeyframe(deadId, glm::vec2(2.f, row));

    // Set default animation
    sprite->changeAnimation(walkId);


    setPosition(pos * map->getTileSize());
}

void Goomba::update(float deltaTime) {
    if (currentState == State::DEAD) return;
    sprite->update(deltaTime);
    updateVelocity(deltaTime);
    updatePosition(deltaTime);
    setPosition(pos);
}


void Goomba::updateVelocity(float deltaTime) {
    if (map->onGround(pos, enemySize))
        vel.y = 0.f;
    else {
        vel.y += GRAVITY_ACC * deltaTime;
        if (vel.y > FALLING_TERMINAL_VEL)
            vel.y = FALLING_TERMINAL_VEL;
    }

    vel.x = GOOMBA_SPEED * (int)dir;
}

void Goomba::updatePosition(float deltaTime) {
    int xNext = pos.x + (int)(vel.x * deltaTime);
    int yNext = pos.y - (int)(vel.y * deltaTime);

    glm::ivec2 nextPos = glm::ivec2(xNext, yNext);

    if (map->solveCollisionsY(pos, nextPos, enemySize))
        vel.y = 0.f;

    if (map->solveCollisionsX(pos, nextPos, enemySize))
        dir = (Dir)(-(enum_t)dir);

    this->pos = nextPos;
}
