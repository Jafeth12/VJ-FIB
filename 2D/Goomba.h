#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"
#include "TileMap.h"


class Goomba : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Enemy::Color color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) override;
        void update(float deltaTime) override;

        void die() { currentState = State::DEAD; };
        bool isDead() const override { return currentState == State::DEAD; };

        bool collidesWith(const Goomba &other) const;

    private:
        static Texture *s_goombaTexture;

        void updatePosition(float deltaTime);
        void updateVelocity(float deltaTime);

        // Animations
        typedef int enum_t;
        enum class State : enum_t { WALK, DEAD };

        enum_t getAnimId(State a) const { return (enum_t)a; };

        // Attributes
        State currentState;
};

#endif
