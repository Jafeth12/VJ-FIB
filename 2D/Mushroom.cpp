#include "Mushroom.h"
#include <iostream>

#define MUSH_SIZE_IN_SPRITESHEET 16.f

Mushroom::Mushroom() {}

Mushroom::Mushroom(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld) {
    init(tileMapPos, shaderProgram, map, overworld);
}

void Mushroom::init(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld) {
    Powerup::init(tileMapPos, shaderProgram, map);

    texture.loadFromFile("images/seta.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texture.setMagFilter(GL_NEAREST);
    texture.setMinFilter(GL_NEAREST);

    glm::vec2 size = glm::vec2(MUSH_SIZE_IN_SPRITESHEET/texture.width(), MUSH_SIZE_IN_SPRITESHEET/texture.height());
    sprite = Sprite::createSprite(glm::vec2(32.f), size, &texture, &shaderProgram);
    sprite->setPosition(pos);
}

