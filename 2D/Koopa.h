#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Enemy.h"
#include "TileMap.h"

class Koopa : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) override;
        void update(float deltaTime) override;

        void becomeShell() { currentState = State::SHELL; vel.x = 0.f; dir = Dir::NONE; };
        void kick(Dir dir);
        void dieLateral() override { currentState = State::INVERTED; Enemy::dieLateral(); };
        void dieVertical() override;
        void dieFall() override { currentState = State::DEAD; dir = Dir::NONE; Enemy::dieFall(); };

        bool isDead() const override { return currentState == State::DEAD; };
        bool isDying() const override { return currentState == State::INVERTED; };
        bool isShell() const { return currentState == State::SHELL; };
        bool isMovingShell() const { return currentState == State::SHELL && vel.x != 0.f; };

    private:
        static Texture *s_koopaTexture;

        void updatePosition(float deltaTime);
        void updateVelocity(float deltaTime);
        void updateAnimation();

        // Animations
        typedef int enum_t;
        enum class State : enum_t { WALK, SHELL, INVERTED, DEAD };
        enum_t getAnimId(State a) const { return (enum_t)a + 1; };
        enum_t getAnimId(State a, Dir d) const { if (a == State::WALK) return (d == Dir::LEFT) ? (enum_t)a : (enum_t)a+1; else return getAnimId(a); };

        // Attributes
        State currentState;
};

#endif
