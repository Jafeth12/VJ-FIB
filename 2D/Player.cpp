#include <cmath>
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define SPEED 16 //FIXME SPEED -> ANIM_SPEED
#define PLAYER_SIZE glm::ivec2(16, 16)
#define JUMP_HEIGHT 115.f
#define JUMP_TIME .5f
#define N_FALL_GRAVITY 3.f
#define GRAVITY_ACC ((-2*JUMP_HEIGHT)/(JUMP_TIME*JUMP_TIME))
#define JUMP_VEL sqrtf(-2.f * GRAVITY_ACC * JUMP_HEIGHT)

#define X_TOP_SPEED 500.f
#define X_ACC  500.f
#define X_DRAG 500.f

enum PlayerAnims
{
	STAND_LEFT,
    STAND_RIGHT,
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP_LEFT,
    JUMP_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
    spritesheet.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    // spritesheet.width() = spritesheet.height() = 128
    // cada sprite es de 16x16, así que el size es 16/128 = 0.125

    float size = 0.125f;
	sprite = Sprite::createSprite(PLAYER_SIZE, glm::vec2(size, size), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
    velPlayer = glm::vec2(0.0f);
    yState = FLOOR;
	
		sprite->setAnimationSpeed(STAND_LEFT, SPEED);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f * size, 0.f * size));
		
		sprite->setAnimationSpeed(STAND_RIGHT, SPEED);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f * size, 1.f * size));
		
		sprite->setAnimationSpeed(MOVE_LEFT, SPEED);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3.f * size, 1.f * size));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2.f * size, 1.f * size));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1.f * size, 1.f * size));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, SPEED);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3.f * size, 0.f * size));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2.f * size, 0.f * size));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1.f * size, 0.f * size));

        sprite->setAnimationSpeed(JUMP_LEFT, SPEED);
        sprite->addKeyframe(JUMP_LEFT, glm::vec2(5.f * size, 1.f * size));

        sprite->setAnimationSpeed(JUMP_RIGHT, SPEED);
        sprite->addKeyframe(JUMP_RIGHT, glm::vec2(5.f * size, 0.f * size));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(float deltaTime)
{
    // Animations and stuff
	sprite->update(deltaTime);

    glm::vec2 acc = glm::vec2(0.f);
    bool leftPressed = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
    bool rightPressed = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);

    // Update X accelerattion and animation
	if (leftPressed) {
        // Apply acceleration
        acc.x = -X_ACC;
        // Change animation
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
	} else if (rightPressed) {
        // Apply acceleration
        acc.x = X_ACC;
        // Change animation
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
	} else { // No lateral key pressed
        // Apply drag
        if (velPlayer.x > 0.f) acc.x = -X_DRAG;
        else if (velPlayer.x < 0.f) acc.x = X_DRAG;
        // Change animation
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

    // Update Y position of the player
    bool upPressed = Game::instance().getSpecialKey(GLUT_KEY_UP);
    bool onGround = map->onGround(posPlayer, PLAYER_SIZE);
    bool headUnderTile = map->headUnderTile(posPlayer, PLAYER_SIZE);

    // Change the current state, based on a couple variables
    updateYState(upPressed, onGround, headUnderTile);

    // Change vars based on the state
    switch (yState) {
        case FLOOR:
            acc.y = 0.f;
            velPlayer.y = 0.f;
            if (!upPressed) {
                bJumping = false;
            }
            break;

        case UPWARDS:
            acc.y = GRAVITY_ACC;
            if (upPressed && onGround && !bJumping) {
                velPlayer.y = JUMP_VEL;
                bJumping = true;
            }
            break;

        case DOWNWARDS:
            acc.y = N_FALL_GRAVITY * GRAVITY_ACC;
            break;

        default:
            break;
    }

    if (yState == UPWARDS || yState == DOWNWARDS) {
        if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(JUMP_LEFT);
        else if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
        else if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(JUMP_LEFT);
        else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
    }
    else {
        if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(STAND_LEFT);
        else if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(STAND_RIGHT);
    }
    
    // Act uppon state and the vars
    updateVelocity(acc, deltaTime);
    updatePosition(deltaTime);

	// Set the new position of the player
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition() {
    return posPlayer;
}

void Player::updateVelocity(glm::vec2 acc, float deltaTime)
{
    // Update and limit X
    velPlayer.x += acc.x * deltaTime;
    if (velPlayer.x < -X_TOP_SPEED) velPlayer.x = -X_TOP_SPEED;
    if (velPlayer.x > X_TOP_SPEED) velPlayer.x = X_TOP_SPEED;

    // Update and limit Y
    velPlayer.y += acc.y * deltaTime;
}

void Player::updatePosition(float deltaTime)
{
    // Update X
    // Calculate teoretical next position
    int xNextPos = posPlayer.x + int(velPlayer.x * deltaTime);
    glm::ivec2 nextPos = glm::ivec2(xNextPos, posPlayer.y);
    // Check for collisions
    bool collisions = false;
    if (velPlayer.x < 0.f) collisions = map->collisionMoveLeft(nextPos, PLAYER_SIZE);
    else if (velPlayer.x > 0.f) collisions = map->collisionMoveRight(nextPos, PLAYER_SIZE);
    // Make the player come to a stop if there was a collision
    if (collisions) velPlayer.x = 0.0f;
    // Only apply the new position if there was no collision
    else posPlayer.x = xNextPos;

    // Update Y
    // We only check for collisions if we are not on the floor
    if (yState != FLOOR) {
        // Calculate theoretical next position
        int yNextPos = posPlayer.y - int(velPlayer.y * deltaTime);
        glm::ivec2 nextPos = glm::ivec2(posPlayer.x, yNextPos);
        // Correct the position if we collide
        if(map->collidesWithMap(posPlayer, &nextPos, PLAYER_SIZE))
            // Make the player come to a stop if there was a collision
            velPlayer.y = 0.0f;
        // Apply the new position
        posPlayer.y = nextPos.y;
    }
}

void Player::updateYState(bool upPressed, bool onGround, bool headUnderTile)
{
    switch (yState)
    {
        case FLOOR:
            if (onGround && upPressed && !bJumping)
                yState = UPWARDS;
            if (!onGround)
                yState = DOWNWARDS;
            break;

        case UPWARDS:
            if (!upPressed || velPlayer.y <= 0.f || headUnderTile)
                yState = DOWNWARDS;
            break;

        case DOWNWARDS:
            if (onGround)
                yState = FLOOR;
            break;
    }
}
