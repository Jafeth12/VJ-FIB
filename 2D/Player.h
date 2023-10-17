#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Camera.h"
#include "Sprite.h"
#include "TileMap.h"
#include <glm/fwd.hpp>

enum PlayerYState { FLOOR, UPWARDS, DOWNWARDS };
enum PlayerXState { RUN_LEFT, WALK_LEFT, NONE, WALK_RIGHT, RUN_RIGHT };

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

    void updateVelocity(glm::vec2 acc, bool shouldJump, float deltaTime);
    void updatePosition(float deltaTime);
    bool updateYState(bool upPressed);
    void updateXState(bool leftPressed, bool rightPressed, bool runPressed);
    void updateAnimation(bool leftPressed, bool rightPressed);
    glm::vec2 getAcceleration();

	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

    PlayerYState yState;
    PlayerXState xState;

    glm::vec2 velPlayer;
};


#endif // _PLAYER_INCLUDE
