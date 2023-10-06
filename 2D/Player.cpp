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

    // Update the X position of the player
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, PLAYER_SIZE))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, PLAYER_SIZE))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
    // Update Y position of the player
    bool upPressed = Game::instance().getSpecialKey(GLUT_KEY_UP);
    bool onGround = map->onGround(posPlayer, PLAYER_SIZE);
    bool headUnderTile = map->headUnderTile(posPlayer, PLAYER_SIZE);
    float g = 0.f;

    // Change the current state, based on a couple variables
    updateYState(upPressed, onGround, headUnderTile);

    // Change vars based on the state
    switch (yState) {
        case FLOOR:
            g = 0.f;
            velPlayer.y = 0.f;
            if (!upPressed) {
                bJumping = false;
            }
            break;

        case UPWARDS:
            g = GRAVITY_ACC;
            if (upPressed && onGround && !bJumping) {
                velPlayer.y = JUMP_VEL;
                bJumping = true;
            }
            break;

        case DOWNWARDS:
            g = N_FALL_GRAVITY * GRAVITY_ACC;
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
    updateVelocity(glm::vec2(0.f, g), deltaTime);
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
    velPlayer += acc.y * deltaTime;
}

void Player::updatePosition(float deltaTime)
{
    if (yState == DOWNWARDS || yState == UPWARDS) {
        int yNextPos = posPlayer.y - int(velPlayer.y * deltaTime);
        glm::ivec2 nextPos = glm::ivec2(posPlayer.x, yNextPos);
        if(map->collidesWithMap(posPlayer, &nextPos, PLAYER_SIZE))
            velPlayer.y = 0.0f;

        posPlayer.y = nextPos.y;
    }
}

void Player::updateYState(bool upPressed, bool onGround, bool headUnderTile)
{
    switch (yState)
    {
        case FLOOR:
            if (onGround && upPressed)
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
