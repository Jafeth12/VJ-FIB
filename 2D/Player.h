#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Camera.h"
#include "Sprite.h"
#include "TileMap.h"
#include <glm/fwd.hpp>

enum PlayerVerticalState { FLOOR, UPWARDS, DOWNWARDS };

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

    void updateVelocity(glm::vec2 acc, float deltaTime);
    void updatePosition(float deltaTime);
    void updateYState(bool upPressed, bool onGround);

	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

    PlayerVerticalState yState;
    glm::vec2 velPlayer;
};


#endif // _PLAYER_INCLUDE
