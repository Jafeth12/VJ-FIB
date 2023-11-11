#include "Star.h"
#include <iostream>

#define STAR_SIZE_IN_SPRITESHEET 16.f

Star::Star() : Powerup() {

}

Star::~Star() {

}

Star::Star(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld) {
    init(tileMapPos, shaderProgram, map, overworld);
}

void Star::init(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld) {
    Powerup::init(tileMapPos, shaderProgram, map);

    texture.loadFromFile("images/estrellitas.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texture.setMagFilter(GL_NEAREST);
    texture.setMinFilter(GL_NEAREST);

    glm::vec2 size = glm::vec2(STAR_SIZE_IN_SPRITESHEET/texture.width(), STAR_SIZE_IN_SPRITESHEET/texture.height());
    sprite = Sprite::createSprite(POWERUP_SIZE, size, &texture, &shaderProgram);
    sprite->setPosition(pos);

    sprite->setNumberAnimations(2);

    // underworld
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(1, glm::vec2(1.f, 0.f));
    sprite->addKeyframe(1, glm::vec2(1.f, 1.f));
    sprite->addKeyframe(1, glm::vec2(1.f, 2.f));
    sprite->addKeyframe(1, glm::vec2(1.f, 3.f));

    // overworld
    sprite->setAnimationSpeed(1, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.f, 1.f));
    sprite->addKeyframe(0, glm::vec2(0.f, 2.f));
    sprite->addKeyframe(0, glm::vec2(0.f, 3.f));

    sprite->changeAnimation(overworld);
}

