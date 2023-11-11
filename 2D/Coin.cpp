#include <iostream>
#include "Coin.h"
#include "TileMap.h"


Coin::Coin(ShaderProgram &shaderProgram, glm::ivec2 pos, TileMap *map) {
    init(shaderProgram, pos, map);
}

void Coin::init(ShaderProgram &shaderProgram, glm::ivec2 pos, TileMap *map) {

    this->map = map;
    this->texture = map->getTexture();
    

    float sizeX = 32.f / texture->width();
    float sizeY = 32.f / texture->height();

    sprite = Sprite::createSprite(glm::ivec2(getSize()), glm::vec2(sizeX, sizeY), texture, &shaderProgram);


    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);

    if (map->getMapColor() == TileMap::MapColor::OVERWORLD) {
        sprite->addKeyframe(0, glm::vec2(11.f, 1.f));
        sprite->addKeyframe(0, glm::vec2(12.f, 1.f));
        sprite->addKeyframe(0, glm::vec2(13.f, 1.f));
    }
    else {
        sprite->addKeyframe(0, glm::vec2(9.f, 2.f));
        sprite->addKeyframe(0, glm::vec2(10.f, 2.f));
        sprite->addKeyframe(0, glm::vec2(11.f, 2.f));
    }

    sprite->changeAnimation(0);

    pos.x *= map->getTileSize();
    pos.y *= map->getTileSize();
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
