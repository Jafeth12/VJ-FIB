#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Camera.h"
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
	void update(float deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

    glm::ivec2 getPosition() const;
    glm::ivec2 getSize() const;

    bool collidesWithEnemy(const Enemy &enemy) const;

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

private:
    // Animations
    typedef short enum_t;
    enum class VerticalAnim : enum_t { WALK, RUN, SPRINT, STAND, JUMP, BRAKE, _LAST };
    enum class LateralAnim  : enum_t { LEFT, RIGHT, _LAST };
    enum class SpecialAnim  : enum_t { DIE, _LAST };

    const int numAnims = (int)getAnimId(SpecialAnim::_LAST);
    int getAnimId(VerticalAnim v, LateralAnim l) const { return (enum_t)l * (enum_t)VerticalAnim::_LAST + (enum_t)v; }
    int getAnimId(SpecialAnim s) const { return (enum_t)s + (enum_t)VerticalAnim::_LAST * (enum_t)LateralAnim::_LAST; }
    VerticalAnim getVerticalAnim(int a) const { return (VerticalAnim)((enum_t)a % (enum_t)VerticalAnim::_LAST); };
    LateralAnim getLateralAnim(int a) const { return (LateralAnim)((enum_t)a / (enum_t)VerticalAnim::_LAST); };
    void updateAnimation(bool leftPressed, bool rightPressed) const;

    // Physics
    enum PlayerYState { FLOOR, UPWARDS, DOWNWARDS };
    enum PlayerXState { RUN_LEFT, WALK_LEFT, NONE, WALK_RIGHT, RUN_RIGHT };

    void updateVelocity(glm::vec2 acc, bool shouldJump, float deltaTime);
    void updatePosition(float deltaTime);

    void updatePlayerState(float deltaTime);
    bool updateYState(bool upPressed);
    void updateXState(bool leftPressed, bool rightPressed, bool runPressed);
    glm::vec2 getAcceleration();

	bool bJumping;
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

    PlayerYState yState;
    PlayerXState xState;

    glm::ivec2 posPlayer;
    glm::vec2 velPlayer;

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
