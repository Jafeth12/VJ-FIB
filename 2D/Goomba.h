#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"

class Goomba : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, EnemyColor color);
        void update(float deltaTime);
        void render();

        void setTileMap(TileMap *tileMap);
        void setPosition(const glm::vec2 &pos);

    private:
        // Animations
        typedef int enum_t;
        enum class Dir : enum_t { LEFT=-1, RIGHT=1, NONE=0 };
        enum class Anim : enum_t { WALK, DEAD };

        enum_t getAnimId(Anim a) const { return (enum_t)a; };

        // Attributes
        Dir goombaDir;
};

#endif
