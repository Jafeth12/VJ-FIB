#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_HEIGHT 96.f
#define JUMP_TIME .75f
#define GRAVITY_ACC ((-2*JUMP_HEIGHT)/(JUMP_TIME*JUMP_TIME))


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
    yVelocity = 0.0f;
    yState = FLOOR;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
    // Animations and stuff
	sprite->update(deltaTime);

    // Update the X position of the player
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
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
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
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
    float deltaTimef = float(deltaTime) / 1000.f;
    bool upPressed = Game::instance().getSpecialKey(GLUT_KEY_UP);
    bool collisionDown = map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
    float g;

    // Actu uppon the state of the player
    switch (yState)
    {
        case FLOOR:
            g = 0.f;
            yVelocity = 0.f;
            break;

        case UPWARDS:
            g = GRAVITY_ACC;
            if (upPressed && collisionDown)
                yVelocity = sqrtf(-2.f * GRAVITY_ACC * JUMP_HEIGHT);
            break;

        case DOWNWARDS:
            g = 3 * GRAVITY_ACC;
            break;

        default:
            break;
    }
    
    std::cout << "CD: " << collisionDown << " | PUP: " << upPressed << " | " << ((yState == 0) ? "FLOOR" : ((yState == 1) ? "UPWARDS" : "DOWNWARDS")) << std::endl;

    // Update the player state
    posPlayer.y -= int(yVelocity * deltaTimef);
    yVelocity += g * deltaTimef;

    // Change to next state, once the variables have been updated
    switch (yState)
    {
        case FLOOR:
            if (upPressed && collisionDown)
                yState = UPWARDS;
            if (!collisionDown)
                yState = DOWNWARDS;
            break;

        case UPWARDS:
            if (!upPressed || yVelocity < 0.f)
                yState = DOWNWARDS;
            break;

        case DOWNWARDS:
            if (collisionDown)
                yState = FLOOR;
            break;
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
