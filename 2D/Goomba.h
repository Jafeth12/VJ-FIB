#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"
#include "TileMap.h"

class Goomba : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &initPos);
        void update(float deltaTime);

    private:
        void updatePosition(float deltaTime);
        void updateVelocity(float deltaTime);

        // Animations
        typedef int enum_t;
        enum class Anim : enum_t { WALK, DEAD };

        enum_t getAnimId(Anim a) const { return (enum_t)a; };

        // Attributes
        Dir dir;
        glm::vec2 vel;
};

#endif
