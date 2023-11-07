#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

// Generic enemy class
class Enemy {

    public:
        enum class EnemyColor { OVERWORLD, UNDERWORLD };

        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, EnemyColor color);
        void update(float deltaTime);
        void render();
        void setTileMap(TileMap *tileMap);
        void setPosition(const glm::vec2 &pos);

        Texture spritesheet;
        Sprite *sprite;
        TileMap *map;

        glm::ivec2 tileMapDispl,
                   pos;

};

#endif // _ENEMY_INCLUDE
