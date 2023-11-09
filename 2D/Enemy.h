#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

// Generic enemy class
class Enemy {

    public:
        typedef int enum_t;
        enum class Color : enum_t { OVERWORLD, UNDERWORLD };
        enum class Dir : enum_t { LEFT=-1, RIGHT=1, NONE=0 };

        void render();
        glm::ivec2 getPosition() const { return pos; };
        virtual bool isDead() const { return true; };

    protected:
        virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) {};
        virtual void update(float deltaTime) {};

        void setPosition(const glm::vec2 &pos);

        Texture spritesheet;
        Sprite *sprite;
        TileMap *map;

        glm::ivec2 tileMapDispl,
                   pos;
        Dir dir;
        glm::vec2 vel;

};

#endif // _ENEMY_INCLUDE
