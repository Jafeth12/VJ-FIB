#include "Interrogation.h"
#include "TileMap.h"

Interrogation::Interrogation(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color, BlockContent content) :
    InteractiveBlock(tileMapDispl, map, pos, shaderProgram, texture, color)
{
    this->content = content;

    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);

    if (color == TileMap::MapColor::UNDERWORLD) {
        sprite->addKeyframe(0, glm::vec2(5.f, 2.f));
        sprite->addKeyframe(0, glm::vec2(6.f, 2.f));
        sprite->addKeyframe(0, glm::vec2(7.f, 2.f));
    }
    else {
        sprite->addKeyframe(0, glm::vec2(12.f, 0.f));
        sprite->addKeyframe(0, glm::vec2(13.f, 0.f));
        sprite->addKeyframe(0, glm::vec2(14.f, 0.f));
    }

    sprite->changeAnimation(0);
}

Interrogation::~Interrogation() { }

void Interrogation::activate() {
    state = BlockState::OFF;
    map->placeTile(getTile(), (int)'4'-'0');
}

void Interrogation::render() {
    InteractiveBlock::render();
}
