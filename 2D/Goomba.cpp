#include "Goomba.h"
#include "Enemy.h"

#define ANIM_SPEED 8
#define GOOMBA_SIZE_IN_SPRITESHEET 32.f
#define GOOMBA_SIZE glm::ivec2(32, 32)
#define GOOMBA_SPEED 120.f

void Goomba::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Enemy::EnemyColor color) {
    goombaDir = Dir::RIGHT;
    tileMapDispl = tileMapPos;

    spritesheet.loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    float size = GOOMBA_SIZE_IN_SPRITESHEET / spritesheet.width();
    sprite = Sprite::createSprite(GOOMBA_SIZE, glm::vec2(size, size), &spritesheet, &shaderProgram);

    float row;
    if (color == EnemyColor::OVERWORLD)
        row = 0.f;
    else // UNDERWORLD
        row = 1.f;

    // Only 2 animations
    sprite->setNumberAnimations(2);

    enum_t walkId = getAnimId(Anim::WALK);
    enum_t deadId = getAnimId(Anim::DEAD);

    // WALK
    sprite->setAnimationSpeed(walkId, ANIM_SPEED);
    sprite->addKeyframe(walkId, glm::vec2(0.f, row));
    sprite->addKeyframe(walkId, glm::vec2(1.f, row));

    // DEAD
    sprite->setAnimationSpeed(deadId, ANIM_SPEED);
    sprite->addKeyframe(deadId, glm::vec2(2.f, row));

    // Set default animation
    sprite->changeAnimation(getAnimId(Anim::WALK));

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Goomba::update(float deltaTime) {
    sprite->update(deltaTime);
    updatePosition(deltaTime);
    setPosition(pos);
}

void Goomba::render() {
    sprite->render();
}

void Goomba::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

void Goomba::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Goomba::updatePosition(float deltaTime) {
    int xNext = pos.x + (int)goombaDir * (int)(GOOMBA_SPEED * deltaTime);
    glm::ivec2 nextPos = glm::ivec2(xNext, pos.y);

    if (map->solveCollisionsX(pos, nextPos, GOOMBA_SIZE))
        goombaDir = (Dir)(-(enum_t)goombaDir);

    this->pos = nextPos;
}
