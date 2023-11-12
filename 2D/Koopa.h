#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Enemy.h"
#include "Player.h"
#include "TileMap.h"

#define KOOPA_SIZE glm::ivec2(32, 48)
#define KOOPA_SHELL_SIZE glm::ivec2(32, 32)

class Koopa : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, TileMap::MapColor color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) override;
        void update(float deltaTime, const glm::ivec2 &playerPos);

        glm::ivec2 getSize() const override { 
            if (currentState == State::SHELL) return KOOPA_SHELL_SIZE; 
            return KOOPA_SIZE; 
        };

        void becomeShell() { currentState = State::SHELL; vel.x = 0.f; dir = Dir::NONE; };
        void kick(Dir dir);
        void dieLateral() override { currentState = State::INVERTED; Enemy::dieLateral(); };
        void dieVertical() override;
        void dieFall() override { currentState = State::DEAD; dir = Dir::NONE; Enemy::dieFall(); };

        void reset() override;

        bool isDead() const override { return currentState == State::DEAD; };
        bool isDying() const override { return currentState == State::INVERTED; };
        bool isShell() const { return currentState == State::SHELL; };
        bool isMovingShell() const { return currentState == State::SHELL && bKicked && timeSinceKick >= 0.5f; };

        Dir kickDirection(Player &p) const;

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
        float timeSinceShell;

        bool bKicked;
        float timeSinceKick;
};

#endif
