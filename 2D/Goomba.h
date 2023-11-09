#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"
#include "TileMap.h"


class Goomba : public Enemy {
    public:
        void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, TileMap::MapColor color, Enemy::Dir initialDirection, const glm::ivec2 &initPos) override;
        void update(float deltaTime) override;

        void dieLateral() override { currentState = State::FLIPPED; Enemy::dieLateral(); };
        void dieVertical() override { currentState = State::CRUSHED; dir = Dir::NONE; Enemy::dieVertical(); };
        void dieFall() override { currentState = State::DEAD; dir = Dir::NONE; Enemy::dieFall(); };

        bool isDead() const override { return currentState == State::DEAD; };
        bool isDying() const override { return currentState == State::FLIPPED || currentState == State::CRUSHED; };

    private:
        static Texture *s_goombaTexture;

        void updatePosition(float deltaTime);
        void updateVelocity(float deltaTime);
        void updateAnimation();


        // Animations
        typedef int enum_t;
        enum class State : enum_t { WALK, CRUSHED, FLIPPED, DEAD };

        enum_t getAnimId(State a) const { return (enum_t)a; };

        // Attributes
        State currentState;
        float timeSinceCrushed;
};

#endif
