#include "Brick.h"
#include "TileMap.h"

Brick::Brick(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color) : InteractiveBlock(tileMapDispl, map, pos, shaderProgram, texture, color)
{
    float row = 0.f;
    if (color == TileMap::MapColor::UNDERWORLD)
        row = 2.f;
    else // OVERWORLD
        row = 0.f;

    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(1.f, row));

    sprite->changeAnimation(0);
}

Brick::~Brick() { }

