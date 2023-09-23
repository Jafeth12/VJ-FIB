#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Camera.h"
#include "Sprite.h"
#include "TileMap.h"

enum PlayerVerticalState { FLOOR, UPWARDS, DOWNWARDS };

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

    glm::vec2 getPosition();
	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

    PlayerVerticalState yState;
    float yVelocity;
};


#endif // _PLAYER_INCLUDE


