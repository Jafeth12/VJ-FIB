#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE

#include "TileMap.h"
#include "Sprite.h"

class Coin
{
    public:
        void init(ShaderProgram &shaderProgram, glm::ivec2 pos, TileMap *map, Texture *texture);
        void update(float deltaTime);
        void render();

        glm::ivec2 getSize() const { return glm::ivec2(8, 16); };
        glm::ivec2 getPos() const { return pos; };

        void setPos(glm::ivec2 pos);



    private:
        Texture *texture;
        Sprite *sprite;
        TileMap *map;

        glm::ivec2 tileMapDispl;
        glm::ivec2 pos;
};
#endif // _COIN_INCLUDE
