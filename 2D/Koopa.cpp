#include "Koopa.h"
#include "Constants.h"

#define ANIM_SPEED 8
#define KOOPA_SIZE glm::ivec2(32, 48)
#define KOOPA_SPEED 120.f

void Koopa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &pos) {
    dir = initialDirection;
    tileMapDispl = tileMapPos;
    map = tileMap;
    enemySize = KOOPA_SIZE;

    // TODO: Cargar spritesheet solamente una vez (no se como XD)
    spritesheet.loadFromFile("images/koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    float sizeX = 32.f / spritesheet.width();
    float sizeY = 64.f / spritesheet.height();
    sprite = Sprite::createSprite(enemySize, glm::vec2(sizeX, sizeY), &spritesheet, &shaderProgram);

    float row;
    if (color == Color::OVERWORLD)
        row = 0.f;
    else // UNDERWORLD
        row = 1.f;

    // Only 2 animations
    sprite->setNumberAnimations(4);

    enum_t walkL = getAnimId(State::WALK, Dir::LEFT);
    enum_t walkR = getAnimId(State::WALK, Dir::RIGHT);
    enum_t shellId = getAnimId(State::SHELL);
    enum_t deadId = getAnimId(State::DEAD);

    // WALKL
    sprite->setAnimationSpeed(walkL, ANIM_SPEED);
    sprite->addKeyframe(walkL, glm::vec2(0.f, row));
    sprite->addKeyframe(walkL, glm::vec2(1.f, row));

    // WALKR
    sprite->setAnimationSpeed(walkR, ANIM_SPEED);
    sprite->addKeyframe(walkR, glm::vec2(2.f, row));
    sprite->addKeyframe(walkR, glm::vec2(3.f, row));

    // SHELL
    sprite->setAnimationSpeed(shellId, ANIM_SPEED);
    sprite->addKeyframe(shellId, glm::vec2(4.f, row));

    // DEAD
    sprite->setAnimationSpeed(deadId, ANIM_SPEED);
    sprite->addKeyframe(deadId, glm::vec2(5.f, row));

    // Set default animation
    currentState = State::WALK;
    if (dir == Dir::LEFT)
        sprite->changeAnimation(walkL);
    else
        sprite->changeAnimation(walkR);


    setPosition(pos * map->getTileSize());
}

void Koopa::update(float deltaTime) {
    sprite->update(deltaTime);
    updateVelocity(deltaTime);
    updatePosition(deltaTime);
    setPosition(pos);
}


void Koopa::updateVelocity(float deltaTime) {
    if (map->onGround(pos, enemySize))
        vel.y = 0.f;
    else {
        vel.y += GRAVITY_ACC * deltaTime;
        if (vel.y > FALLING_TERMINAL_VEL)
            vel.y = FALLING_TERMINAL_VEL;
    }

    vel.x = KOOPA_SPEED * (int)dir;
}

void Koopa::updatePosition(float deltaTime) {
    int xNext = pos.x + (int)(vel.x * deltaTime);
    int yNext = pos.y - (int)(vel.y * deltaTime);

    glm::ivec2 nextPos = glm::ivec2(xNext, yNext);

    if (map->solveCollisionsY(pos, nextPos, enemySize))
        vel.y = 0.f;

    if (map->solveCollisionsX(pos, nextPos, enemySize)) {
        dir = (Dir)(-(enum_t)dir);
        sprite->changeAnimation(getAnimId(currentState, dir));
    }

    this->pos = nextPos;
}
