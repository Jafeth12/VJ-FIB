#include <iostream>
#include "Coin.h"



void Coin::init(ShaderProgram &shaderProgram, glm::ivec2 pos, TileMap *map, Texture *texture) {

    this->map = map;

    // texture = new Texture();
    // texture->loadFromFile("images/coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
    // texture->setMagFilter(GL_NEAREST);
    // texture->setMinFilter(GL_NEAREST);

    this->texture = texture;

    float sizeX = 8.f / texture->width(),
          sizeY = 16.f / texture->height();

    sprite = Sprite::createSprite(glm::ivec2(getSize()), glm::vec2(sizeX, sizeY), texture, &shaderProgram);

    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(0, glm::vec2(1.f, 0.f));
    sprite->addKeyframe(0, glm::vec2(2.f, 0.f));
    sprite->addKeyframe(0, glm::vec2(3.f, 0.f));

    sprite->changeAnimation(0);

    setPos(pos);
}

void Coin::update(float deltaTime) {
    sprite->update(deltaTime);
}

void Coin::render() {
    sprite->render();
}

void Coin::setPos(glm::ivec2 pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}
