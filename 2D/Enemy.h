#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

// Generic enemy class
class Enemy {

    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
        void update(float deltaTime);
        void render();
        void setTileMap(TileMap *tileMap);

    private:
        Texture spritesheet;
        Sprite *sprite;
        TileMap *map;

        glm::ivec2 tileMapDispl,
                   pos;

};

#endif // _ENEMY_INCLUDE
