#ifndef _INTERROGATION_INCLUDE
#define _INTERROGATION_INCLUDE

#include "InteractiveBlock.h"
#include "TileMap.h"

class Interrogation : public InteractiveBlock
{
    public:
        enum class BlockContent { NONE, COIN, MUSHROOM, STAR };

        Interrogation(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color, BlockContent content);

        void render() override;
        void activate() override;
        ~Interrogation() override;

    private:
        BlockContent content;
};

#endif // _INTERROGATION_INCLUDE
