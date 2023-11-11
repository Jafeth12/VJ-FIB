#include "Powerup.h"

Powerup::Powerup() {
    sprite = NULL;
    map = NULL;
    shaderProgram = NULL;
}

Powerup::~Powerup() {
    if (sprite != NULL)
        delete sprite;
}

void Powerup::init(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map) {
    this->map = map;
    this->shaderProgram = &shaderProgram;
    tileMapPos.x *= map->getTileSize();
    tileMapPos.y *= map->getTileSize();

    pos = tileMapPos;

    dir = Dir::RIGHT;
    vel = glm::vec2(0, 0);

    consumed = false;
    active = false;

}

void Powerup::update(float deltatime) {
    if (!canBeConsumed()) return;

    updateVelocity(deltatime);
    updatePosition(deltatime);

    if (sprite == NULL) return;

    sprite->update(deltatime);
    setPosition(pos);
}

void Powerup::render() {
    if (canBeConsumed()) {
        sprite->render();
    }
}

void Powerup::activate() {
    active = true;
}

void Powerup::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(pos.x, pos.y + 16));
}

void Powerup::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

glm::ivec2 Powerup::getPos() const {
    return pos;
}

glm::vec2 Powerup::getVel() const {
    return vel;
}

bool Powerup::isConsumed() const {
    return consumed;
}

void Powerup::consume() {
    if (!canBeConsumed()) return;

    consumed = true;
}

// PHYSICS

void Powerup::updateVelocity(float deltaTime) {
    if (!canBeConsumed()) return;

    vel.y += GRAVITY_ACC * deltaTime;
    if (vel.y > FALLING_TERMINAL_VEL)
        vel.y = FALLING_TERMINAL_VEL;

    vel.x = POWERUP_SPEED * (int)dir;
}

void Powerup::updatePosition(float deltaTime) {
    if (consumed) return;

    int xNext = pos.x + (int)(vel.x * deltaTime);
    int yNext = pos.y - (int)(vel.y * deltaTime);

    glm::ivec2 nextPos = glm::ivec2(xNext, yNext);

    if (map->solveCollisionsY(pos, nextPos, POWERUP_SIZE))
        vel.y = 0.f;

    if (map->solveCollisionsX(pos, nextPos, POWERUP_SIZE))
        dir = (Dir)(-(char)dir);

    this->pos = nextPos;
}
