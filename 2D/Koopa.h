#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Enemy.h"
#include "TileMap.h"

class Koopa : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) override;
        void update(float deltaTime) override;

        void becomeShell() { currentState = State::SHELL; };
        void die() { currentState = State::DEAD; };

    private:
        void updatePosition(float deltaTime);
        void updateVelocity(float deltaTime);

        // Animations
        typedef int enum_t;
        enum class State : enum_t { WALK, SHELL, DEAD };
        enum_t getAnimId(State a) const { return (enum_t)a + 1; };
        enum_t getAnimId(State a, Dir d) const { if (a == State::WALK) return (d == Dir::LEFT) ? (enum_t)a : (enum_t)a+1; else return getAnimId(a); };

        // Attributes
        State currentState;
};

#endif
