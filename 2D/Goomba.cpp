#include "Goomba.h"
#include "Enemy.h"

#define ANIM_SPEED 8
#define GOOMBA_SIZE_IN_SPRITESHEET 32.f
#define GOOMBA_SIZE glm::ivec2(32, 32)

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

    // WALK
    sprite->setAnimationSpeed(getAnimId(Anim::WALK), ANIM_SPEED);
    sprite->addKeyframe(getAnimId(Anim::WALK), glm::vec2(0.f, row));
    sprite->addKeyframe(getAnimId(Anim::WALK), glm::vec2(1.f, row));

    // DEAD
    sprite->setAnimationSpeed(getAnimId(Anim::DEAD), ANIM_SPEED);
    sprite->addKeyframe(getAnimId(Anim::DEAD), glm::vec2(2.f, row));

    // Set default animation
    sprite->changeAnimation(getAnimId(Anim::WALK));

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Goomba::render() {
    sprite->render();
}

void Goomba::update(float deltaTime) {
    sprite->update(deltaTime);
    pos.x += (enum_t)goombaDir;
    setPosition(pos);
}

void Goomba::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

void Goomba::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}
