#include "Goomba.h"
#include "Constants.h"
#include "TileMap.h"

#define ANIM_SPEED 8
#define GOOMBA_SIZE_IN_SPRITESHEET 32.f
#define GOOMBA_SPEED 100.f
#define GOOMA_CRUSHED_TIME 0.5f

Texture *Goomba::s_goombaTexture = nullptr;

void Goomba::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, TileMap::MapColor color, Enemy::Dir initialDirection, const glm::ivec2 &pos) {
    dir = initialDirection;
    initialDir = initialDirection;
    tileMapDispl = tileMapPos;
    map = tileMap;
    bActive = false;
    timeSinceCrushed = 0.f;

    if (s_goombaTexture == nullptr) {
        s_goombaTexture = new Texture();
        s_goombaTexture->loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
        s_goombaTexture->setMinFilter(GL_NEAREST);
        s_goombaTexture->setMagFilter(GL_NEAREST);
    }
    spritesheet = s_goombaTexture;

    float size = GOOMBA_SIZE_IN_SPRITESHEET / spritesheet->width();
    sprite = Sprite::createSprite(getSize(), glm::vec2(size, size), s_goombaTexture, &shaderProgram);

    float row;
    if (color == TileMap::MapColor::OVERWORLD)
        row = 0.f;
    else // UNDERWORLD
        row = 1.f;

    sprite->setNumberAnimations(3);

    enum_t walkId = getAnimId(State::WALK);
    enum_t crushedId = getAnimId(State::CRUSHED);
    enum_t flippedId = getAnimId(State::FLIPPED);

    // WALK
    sprite->setAnimationSpeed(walkId, ANIM_SPEED);
    sprite->addKeyframe(walkId, glm::vec2(0.f, row));
    sprite->addKeyframe(walkId, glm::vec2(1.f, row));

    // DEAD
    sprite->setAnimationSpeed(crushedId, ANIM_SPEED);
    sprite->addKeyframe(crushedId, glm::vec2(2.f, row));

    // FLIPPED
    sprite->setAnimationSpeed(flippedId, ANIM_SPEED);
    sprite->addKeyframe(flippedId, glm::vec2(3.f, row));

    // Set default animation
    sprite->changeAnimation(walkId);

    initialPos = pos * map->getTileSize();

    setPosition(initialPos);
}

void Goomba::update(float deltaTime) {
    if (bActive == false)
        return;
    sprite->update(deltaTime);
    updateAnimation();
    updateVelocity(deltaTime);
    updatePosition(deltaTime);
    setPosition(pos);

    if (currentState == State::CRUSHED) {
        timeSinceCrushed += deltaTime;
        if (timeSinceCrushed > GOOMA_CRUSHED_TIME) {
            currentState = State::DEAD;
        }
    }
}

void Goomba::reset() {
    Enemy::reset();
    dir = initialDir;
    setPosition(initialPos);
    currentState = State::WALK;
    timeSinceCrushed = 0.f;
}

void Goomba::updateVelocity(float deltaTime) {
    if (currentState == State::CRUSHED)
        vel.y = 0.f;
    else { 
        vel.y += GRAVITY_ACC * deltaTime;
        if (vel.y > FALLING_TERMINAL_VEL)
            vel.y = FALLING_TERMINAL_VEL;
    }

    if (currentState == State::WALK)
        vel.x = GOOMBA_SPEED * (int)dir;
    else if (currentState == State::CRUSHED)
        vel.x = 0.f;
    // if Flipped, carry on with the previous velocity
}

void Goomba::updatePosition(float deltaTime) {
    int xNext = pos.x + (int)(vel.x * deltaTime);
    int yNext = pos.y - (int)(vel.y * deltaTime);

    glm::ivec2 nextPos = glm::ivec2(xNext, yNext);

    if (!isDying()) { 
        if (map->solveCollisionsY(pos, nextPos, getSize()))
            vel.y = 0.f;

        if (map->solveCollisionsX(pos, nextPos, getSize()))
            dir = (Dir)(-(enum_t)dir);
    }
    this->pos = nextPos;
}

void Goomba::updateAnimation() {
    State prevState = (State)sprite->animation();
    if (currentState != prevState) {
        sprite->changeAnimation(getAnimId(currentState));
    }
}
