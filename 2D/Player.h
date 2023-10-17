#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Camera.h"
#include "Sprite.h"
#include "TileMap.h"
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

    glm::vec2 getPosition();

private:
    // Animations
    enum VerticalAnims { STAND=0, MOVE, JUMP, _LAST };
    enum LateralAnims { LEFT=0, RIGHT };
    enum PlayerAnims {
        STAND_LEFT=0,
        MOVE_LEFT,
        JUMP_LEFT,
        STAND_RIGHT=3,
        MOVE_RIGHT,
        JUMP_RIGHT
    };

    PlayerAnims buildAnim(VerticalAnims v, LateralAnims l) const { return (PlayerAnims)(l * VerticalAnims::_LAST + v); }
    VerticalAnims getVerticalAnim(PlayerAnims a) const { return (VerticalAnims)(a % VerticalAnims::_LAST); };
    LateralAnims getLateralAnim(PlayerAnims a) const { return (LateralAnims)(a / VerticalAnims::_LAST); };
    void updateAnimation(bool leftPressed, bool rightPressed) const;

    // Physics
    enum PlayerYState { FLOOR, UPWARDS, DOWNWARDS };
    enum PlayerXState { RUN_LEFT, WALK_LEFT, NONE, WALK_RIGHT, RUN_RIGHT };

    void updateVelocity(glm::vec2 acc, bool shouldJump, float deltaTime);
    void updatePosition(float deltaTime);
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
};


#endif // _PLAYER_INCLUDE
