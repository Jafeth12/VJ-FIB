#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define SIZE 18
#define SPEED 16
#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


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
	sprite = Sprite::createSprite(glm::ivec2(SIZE, SIZE), glm::vec2(size, size), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
	
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

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(SIZE, SIZE)))
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
		if(map->collisionMoveRight(posPlayer, glm::ivec2(SIZE, SIZE)))
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
	
	if(bJumping)
	{
        if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(JUMP_LEFT);
        else if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
        else if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(JUMP_LEFT);
        else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(JUMP_RIGHT);

		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(SIZE, SIZE), &posPlayer.y);
		}
	}
	else
	{
        if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(STAND_LEFT);
        else if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(STAND_RIGHT);

		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(SIZE, SIZE), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
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




