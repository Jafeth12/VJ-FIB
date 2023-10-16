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

// Components of the animation
enum VerticalAnims
{
    STAND=0,
    MOVE,
    JUMP,
};
enum LateralAnims
{
    LEFT=0,
    RIGHT,
};

// Combination of all of those animations components
// The order is important (un poco bit-hacky, pero funciona)
enum PlayerAnims
{
	STAND_LEFT=0,
    MOVE_LEFT,
    JUMP_LEFT,
    STAND_RIGHT=3,
    MOVE_RIGHT,
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

    // Record state of keys
    bool leftPressed = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
    bool rightPressed = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);
    bool upPressed = Game::instance().getSpecialKey(GLUT_KEY_UP);

    // Change the current vertical state, based on a couple variables
    // Get wether we should jump or not
    bool shouldJump = updateYState(upPressed);

    // Calculate the acceleration
    glm::vec2 acc = getAcceleration(leftPressed, rightPressed);

    // Act uppon state and the vars
    updateVelocity(acc, shouldJump, deltaTime);
    updatePosition(deltaTime);

    // Update the Animations
    updateAnimation(leftPressed, rightPressed);

	// Set the new position of the player
    setPosition(posPlayer);

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

void Player::updateVelocity(glm::vec2 acc, bool shouldJump, float deltaTime)
{
    // Update and limit X
    velPlayer.x += acc.x * deltaTime;
    if (velPlayer.x < -X_TOP_SPEED) velPlayer.x = -X_TOP_SPEED;
    if (velPlayer.x > X_TOP_SPEED) velPlayer.x = X_TOP_SPEED;

    // Update and limit Y
    if (shouldJump) {
        velPlayer.y = JUMP_VEL;
        bJumping = true;
    }
    velPlayer.y += acc.y * deltaTime;
}

void Player::updatePosition(float deltaTime)
{
    // Calculate teoretical next position
    glm::ivec2 next_pos;
    next_pos.x = posPlayer.x + (int)(velPlayer.x * deltaTime);
    next_pos.y = posPlayer.y - (int)(velPlayer.y * deltaTime);
    // Check for collisions and correct the next position accordingly
    bool collisionsx = map->solveCollisionsX(posPlayer, next_pos, PLAYER_SIZE);
    bool collisionsy = map->solveCollisionsY(posPlayer, next_pos, PLAYER_SIZE);
    // Make the player come to a stop if there was a collision per axis
    if (collisionsx) velPlayer.x = 0.0f;
    if (collisionsy) velPlayer.y = 0.0f;
    // Apply the new position
    posPlayer = next_pos;
}

bool Player::updateYState(bool upPressed)
{
    // Get the state of the player
    bool onGround = map->onGround(posPlayer, PLAYER_SIZE);
    bool headUnderTile = map->headUnderTile(posPlayer, PLAYER_SIZE);
    // Declare the return variable
    bool shouldJump = false;
    // Update the state
    switch (yState)
    {
        case FLOOR:
            // Transition to UPWARDS. Player should jump now
            if (onGround && upPressed && !bJumping) {
                yState = UPWARDS;
                shouldJump = true;
                bJumping = true;
            }
            // Transition to DOWNWARDS. Player is falling
            if (!onGround)
                yState = DOWNWARDS;
            // Player is on the floor and not pressing up. Allow jumping again
            if (!upPressed)
                bJumping = false;
            break;

        case UPWARDS:
            // Transition to DOWNWARDS. We reached the apex of the jump OR we hit a tile OR we stopped pressing up
            if (!upPressed || velPlayer.y <= 0.f || headUnderTile)
                yState = DOWNWARDS;
            break;

        case DOWNWARDS:
            // Transition to FLOOR
            if (onGround)
                yState = FLOOR;
            break;
    }
    return shouldJump;
}

void Player::updateAnimation(bool leftPressed, bool rightPressed)
{
    // Figure out components of the current animation
    PlayerAnims currentAnim = (PlayerAnims)sprite->animation();
    VerticalAnims verticalAnim = (VerticalAnims)(currentAnim % 3);
    LateralAnims lateralAnim = (LateralAnims)(currentAnim / 3);
    // Setup components for the next animation
    VerticalAnims nextVerticalAnim = verticalAnim;
    LateralAnims nextLateralAnim = lateralAnim;
    // Figure out next vertical animation
    switch (yState) {
        case FLOOR:
            // Both keys pressed or none pressed. Stand still
            if ((!leftPressed && !rightPressed) || (leftPressed && rightPressed))
                nextVerticalAnim = STAND;
            // Only one key pressed. Move
            else
                nextVerticalAnim = MOVE;
            break;
        case UPWARDS:
        case DOWNWARDS:
            nextVerticalAnim = JUMP;
            break;
        default:
            break;
    }
    // Firgure out animation direction
    if (leftPressed && !rightPressed)
        nextLateralAnim = LEFT;
    else if (rightPressed && !leftPressed)
        nextLateralAnim = RIGHT;
    // Update the animation only if it changed
    PlayerAnims nextAnimation = (PlayerAnims)(nextVerticalAnim + 3 * nextLateralAnim);
    if (nextAnimation != currentAnim)
        sprite->changeAnimation(nextAnimation);
}

glm::vec2 Player::getAcceleration(bool leftPressed, bool rightPressed)
{
    glm::vec2 acc = glm::vec2(0.f);

    // Figure out X acceleration
	if (leftPressed && !rightPressed) acc.x = -X_ACC;
	else if (rightPressed && !leftPressed) acc.x = X_ACC;
    // No lateral key pressed or both pressed at the same time. Apply drag
	else {
        if (velPlayer.x > 0.f) acc.x = -X_DRAG;
        else if (velPlayer.x < 0.f) acc.x = X_DRAG;
	}

    // Figure out Y getAcceleration
    switch (yState) {
        case FLOOR:
            acc.y = 0.f;
            break;
        case UPWARDS:
            acc.y = GRAVITY_ACC;
            break;
        case DOWNWARDS:
            acc.y = N_FALL_GRAVITY * GRAVITY_ACC;
            break;
        default:
            break;
    }

    return acc;
}
