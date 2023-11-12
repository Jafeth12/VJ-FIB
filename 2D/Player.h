#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Camera.h"
#include "Coin.h"
#include "InteractiveBlock.h"
#include "Mushroom.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Enemy.h"
#include <glm/fwd.hpp>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(float deltaTime, float scroll);
	void render();

	void setTileMap(TileMap *tileMap);
    void setBackgroundMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

    glm::ivec2 getPosition() const;
    glm::ivec2 getSize() const;

    bool isDead() const { return statePlayer == State::DEAD; }
    bool isDying() const { return statePlayer == State::DYING; }
    bool isBig() const { return statePlayer == State::BIG || statePlayer == State::BIG_STAR; }
    bool isStar() const { return statePlayer == State::SMALL_STAR || statePlayer == State::BIG_STAR; };

    bool isInvincible() const { return statePlayer == State::SMALL_STAR || statePlayer == State::BIG_STAR || statePlayer == State::JUST_TOOK_DAMAGE; }

    bool collidesWith(const Enemy &enemy) const;
    bool collidesWith(const InteractiveBlock &block) const;
    bool collidesWith(const Coin &coin) const;
    bool collidesWith(const Powerup &powerup) const;

    /**
     * @brief Retorna el ángulo entre la recta
     * entre centros del jugador y el enemigo,
     * y la vertical encima del enemigo.
     * Cuando el jugador está directamente encima,
     * el ángulo = 0
     */
    float collisionAngle(const Enemy &enemy) const;

    void stepOnEnemy();
    void takeDamage();
    void fallDie();

    void takeMushroom();
    void takeStar();

    void makeAlive();
    void makeSmall();
    void makeBig();
    void makeStar();
    void resetVelocity() { velPlayer = glm::vec2(0); }

    int getCurrentStarFrame();
    void moveTo(const glm::vec2 &pos);

    enum class FinishingState { POLE, WALKING_TO_CASTLE, ON_CASTLE };

    bool isOnFinishingState();
    void setIsFinishing(bool isFinishing);

    FinishingState getFinishingState();

private:
    // Animations
    typedef short enum_t;
    enum class VerticalAnim : enum_t { WALK, RUN, SPRINT, STAND, JUMP, BRAKE, CLIMB1, CLIMB2, _LAST };
    enum class LateralAnim  : enum_t { LEFT, RIGHT, _LAST };
    enum class AnimType     : enum_t { NORMAL, STAR, _LAST };
    enum class AnimSize     : enum_t { SMALL, BIG, _LAST };
    enum class SpecialAnim  : enum_t { DIE, _LAST };

    const int NUM_ANIMS = (int)getAnimId(SpecialAnim::_LAST);

    int getAnimId(VerticalAnim v, LateralAnim l, AnimSize as) const;
    int getAnimId(SpecialAnim s) const;

    VerticalAnim getVerticalAnim(int a) const;
    LateralAnim getLateralAnim(int a) const;
    AnimSize getAnimSize(int a) const;

    void updateAnimation(bool leftPressed, bool rightPressed, float deltaTime);

    // Physics
    enum PlayerYState { FLOOR, UPWARDS, DOWNWARDS };
    enum PlayerXState { RUN_LEFT, WALK_LEFT, NONE, WALK_RIGHT, RUN_RIGHT };

    void updateVelocity(glm::vec2 acc, bool shouldJump, float deltaTime);
    void updatePosition(float deltaTime, float scroll);

    void updatePlayerState(float deltaTime);
    bool updateYState(bool upPressed);
    void updateXState(bool leftPressed, bool rightPressed, bool runPressed);
    void updateStarFrame();
    glm::vec2 getAcceleration();

    void updatePoleAnimation(float deltaTime);

    bool isOnAutopilot;
    bool isFinishing;
	bool bJumping;
    glm::ivec2 targetPos;
	glm::ivec2 tileMapDispl;
    Texture spritesheet;

	Sprite *sprite;
	TileMap *map;
    TileMap *backgroundMap;

    PlayerYState yState;
    PlayerXState xState;

    FinishingState finishingState;

    glm::ivec2 posPlayer;
    glm::vec2 velPlayer;

    // Animations
    VerticalAnim verticalAnim;
    LateralAnim lateralAnim;
    AnimSize animSize;
    int currentAnim;
    int currentStarFrame;

    ShaderProgram *shaderProgram;

    enum class State {
        SMALL,
        BIG,
        SMALL_STAR,
        BIG_STAR,
        JUST_TOOK_DAMAGE,
        DYING,
        DEAD,
    };

    void setState(State newState);
    State statePlayer;
    float timeCurrentState;

};


#endif // _PLAYER_INCLUDE
