#include <iostream>
#include "InteractiveBlock.h"
#include "TileMap.h"


InteractiveBlock::InteractiveBlock(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color) :
    shaderProgram(shaderProgram)
{
    this->pos = pos;
    this->tileMapDispl = tileMapDispl;
    this->map = map;
    this->state = BlockState::STATIC;
    
    const auto spriteSize = glm::vec2(32, 32);
    const auto quadSize = glm::ivec2(map->getTileSize());

    float sizeX = spriteSize.x / texture->width();
    float sizeY = spriteSize.y / texture->height();
    sprite = Sprite::createSprite(quadSize, glm::vec2(sizeX, sizeY), texture, &shaderProgram);

    pos.x *= map->getTileSize();
    pos.y *= map->getTileSize();
    setPosition(pos);
}

InteractiveBlock::~InteractiveBlock() {
    delete sprite;
}

void InteractiveBlock::update(float deltaTime) {
    sprite->update(deltaTime);
}

void InteractiveBlock::setPosition(glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void InteractiveBlock::render() {
    sprite->render();
}

void InteractiveBlock::activate() {
    state = BlockState::JUMPING;
}

bool InteractiveBlock::canActivate() const {
    return state == BlockState::STATIC;
}
