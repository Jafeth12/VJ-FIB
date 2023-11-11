#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE

#include "TileMap.h"
#include "Sprite.h"

class Coin
{
    public:
        Coin() {};
        Coin(ShaderProgram &shaderProgram, glm::ivec2 pos, TileMap *map);
        ~Coin() {};

        void init(ShaderProgram &shaderProgram, glm::ivec2 pos, TileMap *map);
        void update(float deltaTime);
        void render();

        glm::ivec2 getSize() const { return glm::ivec2(32, 32); };
        glm::ivec2 getPos() const { return pos; };

        void setPos(glm::ivec2 pos);

        void take();
        bool canTake() const;

    private:
        Texture *texture;
        Sprite *sprite;
        TileMap *map;

        bool isTaken = false;

        glm::ivec2 tileMapDispl;
        glm::ivec2 pos;
};
#endif // _COIN_INCLUDE
