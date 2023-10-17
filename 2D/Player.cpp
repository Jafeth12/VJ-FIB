#include <cmath>
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define SPEED 16 //FIXME SPEED -> ANIM_SPEED
#define PLAYER_SIZE glm::ivec2(32, 32)
#define PLAYER_SIZE_IN_SPRITESHEET 16.f
#define JUMP_HEIGHT 175.f
#define JUMP_TIME .62f
#define N_FALL_GRAVITY 3.f
#define FALLING_TERMINAL_VEL 500.f
#define GRAVITY_ACC ((-2*JUMP_HEIGHT)/(JUMP_TIME*JUMP_TIME))
#define JUMP_VEL sqrtf(-2.f * GRAVITY_ACC * JUMP_HEIGHT)

#define X_WALK_SPEED 225.f
#define X_RUN_SPEED 2.f * X_WALK_SPEED
#define X_ACC  550.f
#define X_DRAG 700.f

// Components of the animation
enum VerticalAnims
{
    STAND=0,
    MOVE,
    JUMP,
    _LAST, // Not an animation. Used to get the number of vertical animations
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

    float size = PLAYER_SIZE_IN_SPRITESHEET / spritesheet.width();
	sprite = Sprite::createSprite(PLAYER_SIZE, glm::vec2(size, size), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
    velPlayer = glm::vec2(0.0f);
    yState = FLOOR;
    xState = NONE;
	
		sprite->setAnimationSpeed(STAND_LEFT, SPEED);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, SPEED);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 1.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, SPEED);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3.f, 1.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2.f, 1.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1.f, 1.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, SPEED);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1.f, 0.f));

        sprite->setAnimationSpeed(JUMP_LEFT, SPEED);
        sprite->addKeyframe(JUMP_LEFT, glm::vec2(5.f, 1.f));

        sprite->setAnimationSpeed(JUMP_RIGHT, SPEED);
        sprite->addKeyframe(JUMP_RIGHT, glm::vec2(5.f, 0.f));
		
	sprite->changeAnimation(STAND_RIGHT);
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
    bool upPressed = Game::instance().getKey(' ');
    bool runPressed = Game::instance().getKey('z');

    // Change the movement state of the player, based on inputs
    bool shouldJump = updateYState(upPressed);
    updateXState(leftPressed, rightPressed, runPressed);

    // Calculate the acceleration
    glm::vec2 acc = getAcceleration();

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
    // Update
    velPlayer += acc * deltaTime;
    // Limit
    if (velPlayer.y < -FALLING_TERMINAL_VEL) velPlayer.y = -FALLING_TERMINAL_VEL;
    // Special cases (jump)
    if (shouldJump) {
        velPlayer.y = JUMP_VEL;
        bJumping = true;
    }
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

void Player::updateXState(bool leftPressed, bool rightPressed, bool runPressed) {
    const bool _none = !(leftPressed ^ rightPressed); // Or both.
    const bool onlyR = !leftPressed && rightPressed;
    const bool onlyL = leftPressed && !rightPressed;
    const bool onAir = yState != FLOOR;

    if (!onAir) { // Estamos en el suelo. Podemos cambiar velocidad y dirección.
        if (runPressed) {
            if (onlyL) xState = RUN_LEFT;
            else if (onlyR) xState = RUN_RIGHT;
            else xState = NONE;
        } else {
            if (onlyL) xState = WALK_LEFT;
            else if (onlyR) xState = WALK_RIGHT;
            else xState = NONE;
        }
    } else { // Estamos en el aire, podemos cambiar la dirección, pero no la velocidad
        switch(xState) {
            case RUN_LEFT:
                if (onlyR) xState = RUN_RIGHT;
                break;
            case RUN_RIGHT:
                if (onlyL) xState = RUN_LEFT;
                break;

            case WALK_LEFT:
                if (onlyR) xState = WALK_RIGHT;
                break;
            case WALK_RIGHT:
                if (onlyL) xState = WALK_LEFT;
                break;

            case NONE:
            {
                if (onlyL) xState = WALK_LEFT;
                else if (onlyR) xState = WALK_RIGHT;
                break;
            }
        }
    }
}

void Player::updateAnimation(bool leftPressed, bool rightPressed)
{
    const int n_vertical_anims = (int)VerticalAnims::_LAST;
    // Figure out components of the current animation
    PlayerAnims currentAnim = (PlayerAnims)sprite->animation();
    VerticalAnims verticalAnim = (VerticalAnims)(currentAnim % n_vertical_anims);
    LateralAnims lateralAnim = (LateralAnims)(currentAnim / n_vertical_anims);
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
    PlayerAnims nextAnimation = (PlayerAnims)(nextVerticalAnim + n_vertical_anims * nextLateralAnim);
    if (nextAnimation != currentAnim)
        sprite->changeAnimation(nextAnimation);
}

glm::vec2 Player::getAcceleration()
{
    glm::vec2 acc = glm::vec2(0.f);

    // Figure out Y acceleration
    switch (xState) {
        case RUN_LEFT:
            if (velPlayer.x > -X_RUN_SPEED) acc.x = -X_ACC;
            else if (velPlayer.x < -X_RUN_SPEED) acc.x = X_DRAG;
            break;
        case WALK_LEFT:
            if (velPlayer.x > -X_WALK_SPEED) acc.x = -X_ACC;
            else if (velPlayer.x < -X_WALK_SPEED) acc.x = X_DRAG;
            break;

        case NONE:
            if (velPlayer.x < 0.f) acc.x = X_DRAG;
            else if (velPlayer.x > 0.f) acc.x = -X_DRAG;
            break;

        case WALK_RIGHT:
            if (velPlayer.x < X_WALK_SPEED) acc.x = X_ACC;
            else if (velPlayer.x > -X_WALK_SPEED) acc.x = -X_DRAG;
            break;
        case RUN_RIGHT:
            if (velPlayer.x < X_RUN_SPEED) acc.x = X_ACC;
            else if (velPlayer.x > -X_RUN_SPEED) acc.x = -X_DRAG;
            break;
    }

    // Figure out Y acceleration
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
