#ifndef _BRICK_INCLUDE
#define _BRICK_INCLUDE

#include "InteractiveBlock.h"
#include "TileMap.h"

class Brick : public InteractiveBlock
{
    public:
        Brick(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color);
        ~Brick() override;
};

#endif // _BRICK_INCLUDE
