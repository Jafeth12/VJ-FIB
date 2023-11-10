#ifndef _INTERROGATION_INCLUDE
#define _INTERROGATION_INCLUDE

#include "InteractiveBlock.h"
#include "TileMap.h"

class Interrogation : public InteractiveBlock
{
    public:
        Interrogation(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color);
        ~Interrogation() override;
};

#endif // _INTERROGATION_INCLUDE
