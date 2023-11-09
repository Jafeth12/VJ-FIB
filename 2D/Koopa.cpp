#include "Koopa.h"
#include "Constants.h"

#define ANIM_SPEED 8
#define KOOPA_SIZE glm::ivec2(32, 48)
#define KOOPA_SPEED 120.f
#define SHELL_SPEED 240.f

Texture *Koopa::s_koopaTexture = nullptr;

void Koopa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &pos) {
    dir = initialDirection;
    tileMapDispl = tileMapPos;
    map = tileMap;
    enemySize = KOOPA_SIZE;
    bActive = false;

    if (s_koopaTexture == nullptr) {
        s_koopaTexture = new Texture();
        s_koopaTexture->loadFromFile("images/koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
        s_koopaTexture->setMinFilter(GL_NEAREST);
        s_koopaTexture->setMagFilter(GL_NEAREST);
    }
    spritesheet = s_koopaTexture;

    float sizeX = 32.f / spritesheet->width();
    float sizeY = 64.f / spritesheet->height();
    sprite = Sprite::createSprite(enemySize, glm::vec2(sizeX, sizeY), s_koopaTexture, &shaderProgram);

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
    enum_t invertedId = getAnimId(State::INVERTED);

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

    // INVERTED
    sprite->setAnimationSpeed(invertedId, ANIM_SPEED);
    sprite->addKeyframe(invertedId, glm::vec2(5.f, row));

    // Set default animation
    currentState = State::WALK;
    if (dir == Dir::LEFT)
        sprite->changeAnimation(walkL);
    else
        sprite->changeAnimation(walkR);

    setPosition(pos * map->getTileSize());
}


void Koopa::dieVertical() {
    if (isShell())
        return; // Un koopa o puede morir verticalmente si esta en estado shell
    else if (currentState == State::WALK)
        becomeShell();

    Enemy::dieVertical();
}

void Koopa::update(float deltaTime) {
    if (bActive == false)
        return;
    sprite->update(deltaTime);
    updateVelocity(deltaTime);
    updatePosition(deltaTime);
    updateAnimation();
    setPosition(pos);
}


void Koopa::updateVelocity(float deltaTime) {
    vel.y += GRAVITY_ACC * deltaTime;
    if (vel.y > FALLING_TERMINAL_VEL)
        vel.y = FALLING_TERMINAL_VEL;

    if (currentState == State::SHELL)
        vel.x = SHELL_SPEED * (int)dir;
    else if (currentState == State::WALK)
        vel.x = KOOPA_SPEED * (int)dir;
    else if (currentState == State::DEAD)
        vel.x = 0.f;
    // si inverted, no tocamos vel.x
}

void Koopa::updatePosition(float deltaTime) {
    int xNext = pos.x + (int)(vel.x * deltaTime);
    int yNext = pos.y - (int)(vel.y * deltaTime);

    glm::ivec2 nextPos = glm::ivec2(xNext, yNext);

    if (!isDying()) {
        if (map->solveCollisionsY(pos, nextPos, enemySize))
            vel.y = 0.f;

        if (map->solveCollisionsX(pos, nextPos, enemySize)) {
            dir = (Dir)(-(enum_t)dir);
            sprite->changeAnimation(getAnimId(currentState, dir));
        }
    }
    this->pos = nextPos;
}

void Koopa::kick(Dir dir) {
    if (currentState == State::SHELL) {
        this->dir = dir;
    }
}

void Koopa::updateAnimation() {
    int animId = sprite->animation();
    int nextAnimId = animId;
    switch (currentState) {
        case State::WALK:
            if (dir == Dir::LEFT)
                nextAnimId = getAnimId(currentState, Dir::LEFT);
            else
                nextAnimId = getAnimId(currentState, Dir::RIGHT);
            break;
        case State::SHELL:
        case State::INVERTED:
        case State::DEAD:
            nextAnimId = getAnimId(currentState);
            break;
    }
    if (animId != nextAnimId)
        sprite->changeAnimation(nextAnimId);
}
