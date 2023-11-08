#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Enemy.h"
#include "TileMap.h"

class Koopa : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &initPos);
        void update(float deltaTime);

    private:
        void updatePosition(float deltaTime);
        void updateVelocity(float deltaTime);

        // Animations
        typedef int enum_t;
        enum class Anim : enum_t { WALK, SHELL, DEAD };

        enum_t getAnimId(Anim a) const { return (enum_t)a + 1; };
        enum_t getAnimId(Anim a, Dir d) const { if (a == Anim::WALK) return (d == Dir::LEFT) ? (enum_t)a : (enum_t)a+1; else return getAnimId(a); };

        // Attributes
        Dir dir;
        glm::vec2 vel;
        Anim currentAnim;
};

#endif
