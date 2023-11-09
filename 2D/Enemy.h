#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

// Generic enemy class
class Enemy {

    public:
        typedef int enum_t;
        enum class Dir : enum_t { LEFT=-1, RIGHT=1, NONE=0 };

        void render();
        glm::ivec2 getPosition() const { return pos; };
        glm::ivec2 getSize() const { return enemySize; };

        virtual bool isDead() const { return true; };
        virtual bool isDying() const { return false; };
        virtual bool shouldCollide() const { return !isDying() && !isDead(); };
        bool collidesWith(const Enemy &other) const;
        bool isActive() const { return bActive; };

        void invertDirection() { dir = (Dir)(-(enum_t)dir); };
        void activate() { bActive = true; };

        virtual void dieLateral() { vel.y = 240.f; };
        virtual void dieVertical() { vel.y = 0.f; };
        virtual void dieFall() { vel.y = 0.f; };


    protected:
        virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, TileMap::MapColor color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) {};
        virtual void update(float deltaTime) {};

        void setPosition(const glm::vec2 &pos);

        Texture *spritesheet;
        Sprite *sprite;
        TileMap *map;

        glm::ivec2 tileMapDispl,
                   pos,
                   enemySize;
        Dir dir;
        glm::vec2 vel;

        bool bActive;

};

#endif // _ENEMY_INCLUDE
