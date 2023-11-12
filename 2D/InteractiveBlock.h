#ifndef _INTERACTIVE_BLOCK_INCLUDE
#define _INTERACTIVE_BLOCK_INCLUDE

#include "Sprite.h"
#include "ShaderProgram.h"
#include "TileMap.h"

class InteractiveBlock
{
    public:
        InteractiveBlock(glm::ivec2 tileMapDispl, TileMap *map, glm::vec2 pos, ShaderProgram &shaderProgram, Texture* texture, TileMap::MapColor color);
        virtual ~InteractiveBlock();

        virtual void update(float deltaTime);
        virtual void render();

        glm::ivec2 getTile() const { return pos / map->getTileSize(); }
        glm::ivec2 getPos() const { return pos; }
        glm::ivec2 getSize() const { return glm::ivec2(32); }

        virtual void activate();

        virtual void reset();

        bool canActivate() const { return state == BlockState::STATIC; }
        bool shouldRender() const { return state != BlockState::OFF; }
    protected:
        enum class BlockState { STATIC, JUMPING, OFF };

        void setPosition(glm::vec2 &pos);

        TileMap *map;
        glm::ivec2  tileMapDispl,
                    pos;
        Sprite *sprite;
        ShaderProgram &shaderProgram;
        BlockState state;
};

#endif // _INTERACTIVE_BLOCK_INCLUDE
