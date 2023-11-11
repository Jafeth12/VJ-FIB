#include "Brick.h"
#include "TileMap.h"

Brick::Brick(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color) : InteractiveBlock(tileMapDispl, map, pos, shaderProgram, texture, color)
{
    glm::vec2 posInSpritesheet = glm::vec2(0.f);
    if (color == TileMap::MapColor::OVERWORLD) {
        posInSpritesheet.x = 1.f;
        posInSpritesheet.y = 0.f;
    } else {
        posInSpritesheet.x = 2.f;
        posInSpritesheet.y = 2.f;
    }

    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, posInSpritesheet);

    sprite->changeAnimation(0);
}

Brick::~Brick() { }

