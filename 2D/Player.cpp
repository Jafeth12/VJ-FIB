#include <cmath>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#include "Constants.h"

#define SPEED 16 //FIXME SPEED -> ANIM_SPEED
#define PLAYER_SIZE glm::ivec2(32, 32)
#define PLAYER_BIG_SIZE glm::ivec2(32, 64)
#define X_WALK_SPEED 225.f
#define X_RUN_SPEED 2.f * X_WALK_SPEED
#define X_ACC  550.f
#define X_DRAG 700.f



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    bJumping = false;
    velPlayer = glm::vec2(0.0f);
    yState = FLOOR;
    xState = NONE;
    statePlayer = State::SMALL;

    spritesheet.loadFromFile("images/all_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
    float sizeX = 16.f / spritesheet.width();
    float sizeY = 32.f / spritesheet.height();
    sprite = Sprite::createSprite(PLAYER_BIG_SIZE, glm::vec2(sizeX, sizeY), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(NUM_ANIMS);

    // spritesheet.width() = spritesheet.height() = 128
    // cada sprite es de 16x16, así que el size es 16/128 = 0.125


    // Unique animation identifiers
    int standR = getAnimId(VerticalAnim::STAND, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::NORMAL);
    int standL = getAnimId(VerticalAnim::STAND, LateralAnim::LEFT, AnimSize::SMALL, AnimType::NORMAL);
    int walkR = getAnimId(VerticalAnim::WALK, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::NORMAL);
    int walkL = getAnimId(VerticalAnim::WALK, LateralAnim::LEFT, AnimSize::SMALL, AnimType::NORMAL);
    int runR = getAnimId(VerticalAnim::RUN, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::NORMAL);
    int runL = getAnimId(VerticalAnim::RUN, LateralAnim::LEFT, AnimSize::SMALL, AnimType::NORMAL);
    int sprintR = getAnimId(VerticalAnim::SPRINT, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::NORMAL);
    int sprintL = getAnimId(VerticalAnim::SPRINT, LateralAnim::LEFT, AnimSize::SMALL, AnimType::NORMAL);
    int jumpR = getAnimId(VerticalAnim::JUMP, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::NORMAL);
    int jumpL = getAnimId(VerticalAnim::JUMP, LateralAnim::LEFT, AnimSize::SMALL, AnimType::NORMAL);
    int brakeR = getAnimId(VerticalAnim::BRAKE, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::NORMAL);
    int brakeL = getAnimId(VerticalAnim::BRAKE, LateralAnim::LEFT, AnimSize::SMALL, AnimType::NORMAL);

    int standR_big = getAnimId(VerticalAnim::STAND, LateralAnim::RIGHT, AnimSize::BIG, AnimType::NORMAL);
    int standL_big = getAnimId(VerticalAnim::STAND, LateralAnim::LEFT, AnimSize::BIG, AnimType::NORMAL);
    int walkR_big = getAnimId(VerticalAnim::WALK, LateralAnim::RIGHT, AnimSize::BIG, AnimType::NORMAL);
    int walkL_big = getAnimId(VerticalAnim::WALK, LateralAnim::LEFT, AnimSize::BIG, AnimType::NORMAL);
    int runR_big = getAnimId(VerticalAnim::RUN, LateralAnim::RIGHT, AnimSize::BIG, AnimType::NORMAL);
    int runL_big = getAnimId(VerticalAnim::RUN, LateralAnim::LEFT, AnimSize::BIG, AnimType::NORMAL);
    int sprintR_big = getAnimId(VerticalAnim::SPRINT, LateralAnim::RIGHT, AnimSize::BIG, AnimType::NORMAL);
    int sprintL_big = getAnimId(VerticalAnim::SPRINT, LateralAnim::LEFT, AnimSize::BIG, AnimType::NORMAL);
    int jumpR_big = getAnimId(VerticalAnim::JUMP, LateralAnim::RIGHT, AnimSize::BIG, AnimType::NORMAL);
    int jumpL_big = getAnimId(VerticalAnim::JUMP, LateralAnim::LEFT, AnimSize::BIG, AnimType::NORMAL);
    int brakeR_big = getAnimId(VerticalAnim::BRAKE, LateralAnim::RIGHT, AnimSize::BIG, AnimType::NORMAL);
    int brakeL_big = getAnimId(VerticalAnim::BRAKE, LateralAnim::LEFT, AnimSize::BIG, AnimType::NORMAL);

    int standR_star = getAnimId(VerticalAnim::STAND, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::STAR);
    int standL_star = getAnimId(VerticalAnim::STAND, LateralAnim::LEFT, AnimSize::SMALL, AnimType::STAR);
    int walkR_star = getAnimId(VerticalAnim::WALK, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::STAR);
    int walkL_star = getAnimId(VerticalAnim::WALK, LateralAnim::LEFT, AnimSize::SMALL, AnimType::STAR);
    int runR_star = getAnimId(VerticalAnim::RUN, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::STAR);
    int runL_star = getAnimId(VerticalAnim::RUN, LateralAnim::LEFT, AnimSize::SMALL, AnimType::STAR);
    int sprintR_star = getAnimId(VerticalAnim::SPRINT, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::STAR);
    int sprintL_star = getAnimId(VerticalAnim::SPRINT, LateralAnim::LEFT, AnimSize::SMALL, AnimType::STAR);
    int jumpR_star = getAnimId(VerticalAnim::JUMP, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::STAR);
    int jumpL_star = getAnimId(VerticalAnim::JUMP, LateralAnim::LEFT, AnimSize::SMALL, AnimType::STAR);
    int brakeR_star = getAnimId(VerticalAnim::BRAKE, LateralAnim::RIGHT, AnimSize::SMALL, AnimType::STAR);
    int brakeL_star = getAnimId(VerticalAnim::BRAKE, LateralAnim::LEFT, AnimSize::SMALL, AnimType::STAR);

    int standR_big_star = getAnimId(VerticalAnim::STAND, LateralAnim::RIGHT, AnimSize::BIG, AnimType::STAR);
    int standL_big_star = getAnimId(VerticalAnim::STAND, LateralAnim::LEFT, AnimSize::BIG, AnimType::STAR);
    int walkR_big_star = getAnimId(VerticalAnim::WALK, LateralAnim::RIGHT, AnimSize::BIG, AnimType::STAR);
    int walkL_big_star = getAnimId(VerticalAnim::WALK, LateralAnim::LEFT, AnimSize::BIG, AnimType::STAR);
    int runR_big_star = getAnimId(VerticalAnim::RUN, LateralAnim::RIGHT, AnimSize::BIG, AnimType::STAR);
    int runL_big_star = getAnimId(VerticalAnim::RUN, LateralAnim::LEFT, AnimSize::BIG, AnimType::STAR);
    int sprintR_big_star = getAnimId(VerticalAnim::SPRINT, LateralAnim::RIGHT, AnimSize::BIG, AnimType::STAR);
    int sprintL_big_star = getAnimId(VerticalAnim::SPRINT, LateralAnim::LEFT, AnimSize::BIG, AnimType::STAR);
    int jumpR_big_star = getAnimId(VerticalAnim::JUMP, LateralAnim::RIGHT, AnimSize::BIG, AnimType::STAR);
    int jumpL_big_star = getAnimId(VerticalAnim::JUMP, LateralAnim::LEFT, AnimSize::BIG, AnimType::STAR);
    int brakeR_big_star = getAnimId(VerticalAnim::BRAKE, LateralAnim::RIGHT, AnimSize::BIG, AnimType::STAR);
    int brakeL_big_star = getAnimId(VerticalAnim::BRAKE, LateralAnim::LEFT, AnimSize::BIG, AnimType::STAR);

    int die = getAnimId(SpecialAnim::DIE);

    // STAND_LEFT
    sprite->setAnimationSpeed(standL, SPEED);
    sprite->addKeyframe(standL, glm::vec2(0.f, 0.f));

    // STAND_RIGHT
    sprite->setAnimationSpeed(standR, SPEED);
    sprite->addKeyframe(standR, glm::vec2(0.f, 1.f));

    // WALK_LEFT
    sprite->setAnimationSpeed(walkL, SPEED);
    sprite->addKeyframe(walkL, glm::vec2(3.f, 1.f));
    sprite->addKeyframe(walkL, glm::vec2(2.f, 1.f));
    sprite->addKeyframe(walkL, glm::vec2(1.f, 1.f));

    // WALK_RIGHT
    sprite->setAnimationSpeed(walkR, SPEED);
    sprite->addKeyframe(walkR, glm::vec2(3.f, 0.f));
    sprite->addKeyframe(walkR, glm::vec2(2.f, 0.f));
    sprite->addKeyframe(walkR, glm::vec2(1.f, 0.f));

    // RUN_LEFT
    sprite->setAnimationSpeed(runL, int(1.5f*float(SPEED)));
    sprite->addKeyframe(runL, glm::vec2(3.f, 1.f));
    sprite->addKeyframe(runL, glm::vec2(2.f, 1.f));
    sprite->addKeyframe(runL, glm::vec2(1.f, 1.f));

    // RUN_RIGHT
    sprite->setAnimationSpeed(runR, int(1.5f*float(SPEED)));
    sprite->addKeyframe(runR, glm::vec2(3.f, 0.f));
    sprite->addKeyframe(runR, glm::vec2(2.f, 0.f));
    sprite->addKeyframe(runR, glm::vec2(1.f, 0.f));

    // SPRINT_LEFT
    sprite->setAnimationSpeed(sprintL, 2*SPEED);
    sprite->addKeyframe(sprintL, glm::vec2(3.f, 1.f));
    sprite->addKeyframe(sprintL, glm::vec2(2.f, 1.f));
    sprite->addKeyframe(sprintL, glm::vec2(1.f, 1.f));

    // SPRINT_RIGHT
    sprite->setAnimationSpeed(sprintR, 2*SPEED);
    sprite->addKeyframe(sprintR, glm::vec2(3.f, 0.f));
    sprite->addKeyframe(sprintR, glm::vec2(2.f, 0.f));
    sprite->addKeyframe(sprintR, glm::vec2(1.f, 0.f));

    // JUMP_LEFT
    sprite->setAnimationSpeed(jumpL, SPEED);
    sprite->addKeyframe(jumpL, glm::vec2(5.f, 1.f));

    // JUMP_RIGHT
    sprite->setAnimationSpeed(jumpR, SPEED);
    sprite->addKeyframe(jumpR, glm::vec2(5.f, 0.f));

    // BRAKE_LEFT
    sprite->setAnimationSpeed(brakeL, SPEED);
    sprite->addKeyframe(brakeL, glm::vec2(4.f, 1.f));

    // BRAKE_RIGHT
    sprite->setAnimationSpeed(brakeR, SPEED);
    sprite->addKeyframe(brakeR, glm::vec2(4.f, 0.f));

    // DIE
    sprite->setAnimationSpeed(die, SPEED);
    sprite->addKeyframe(die, glm::vec2(0.f, 2.f));

    // Default animation
    sprite->changeAnimation(standR);
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

    bool leftShiftPressed = Game::instance().getSpecialKey(112);
    bool rightShiftPressed = Game::instance().getSpecialKey(113);
    bool runPressed = leftShiftPressed || rightShiftPressed;

    // Shortcuts
    if (Game::instance().getKey('S')) { setState(State::SMALL); }
    if (Game::instance().getKey('M')) { setState(State::BIG); }
    if (Game::instance().getKey('G')) {
        if (statePlayer == State::SMALL || statePlayer == State::SMALL_STAR) setState(State::SMALL_STAR);
        else if (statePlayer == State::BIG || statePlayer == State::BIG_STAR) setState(State::BIG_STAR);
    }

    // The real deal. Update the state of the player.
    // Basically based on time.
    updatePlayerState(deltaTime);

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

glm::ivec2 Player::getPosition() const {
    return posPlayer;
}

glm::ivec2 Player::getSize() const {
    if (statePlayer == State::BIG ||
        statePlayer == State::BIG_STAR
        ) return PLAYER_BIG_SIZE;
    else return PLAYER_SIZE;
}

bool Player::collidesWithEnemy(const Enemy &enemy) const {
    glm::ivec2 enemySize = enemy.getSize();
    glm::ivec2 enemyPos = enemy.getPosition();
    glm::ivec2 enemyCenter = enemyPos + enemySize/2;
    glm::ivec2 playerCenter = posPlayer + getSize()/2;
    glm::ivec2 dist = glm::abs(enemyCenter - playerCenter);
    glm::ivec2 minDist = (getSize() + enemySize)/2;
    if (dist.x < minDist.x && dist.y < minDist.y)
        return true;
    return false;
}


float Player::collisionAngle(const Enemy &enemy) const {
    // Puntos de interes:
    //  - p: punto del player
    //  - e: centro del enemigo
    //  - k: punto tal que angulo entre pk y ke sea 90ª
    glm::vec2 p;
    if (statePlayer == State::BIG ||
        statePlayer == State::BIG_STAR)
        p = (glm::vec2)posPlayer + glm::vec2(getSize().x/2.f, (float)(getSize().y)*0.75f);
    else
        p = (glm::vec2)posPlayer + glm::vec2(getSize())/2.f;

    glm::vec2 e = (glm::vec2)enemy.getPosition() + ((glm::vec2)enemy.getSize())/2.f;
    glm::vec2 k = glm::vec2(e.x, p.y);

    float kp = glm::abs(k.x - p.x);
    float ek = glm::abs(e.y - k.y);

    float alpha =glm::atan(kp/ek);
    if (p.y > e.y) alpha += M_PI;

    return alpha;
}

// Animations dark magic
int Player::getAnimId(VerticalAnim v, LateralAnim l, AnimSize as, AnimType t) const {
    return 
        (enum_t)as * (enum_t)AnimType::_LAST * (enum_t)LateralAnim::_LAST * (enum_t)VerticalAnim::_LAST
        +
        (enum_t)t * (enum_t)LateralAnim::_LAST * (enum_t)VerticalAnim::_LAST
        +
        (enum_t)l * (enum_t)VerticalAnim::_LAST
        +
        (enum_t)v;
};

int Player::getAnimId(SpecialAnim s) const {
    return
        (enum_t)AnimSize::_LAST *
        (enum_t)AnimType::_LAST *
        (enum_t)LateralAnim::_LAST *
        (enum_t)VerticalAnim::_LAST
        +
        (enum_t)s;
}

Player::VerticalAnim Player::getVerticalAnim(int a) const {
    return (VerticalAnim)((enum_t)a % (enum_t)VerticalAnim::_LAST);
};
Player::LateralAnim Player::getLateralAnim(int a) const {
    return (LateralAnim)(((enum_t)a / (enum_t)VerticalAnim::_LAST) % (enum_t)LateralAnim::_LAST);
};
Player::AnimType Player::getAnimType(int a) const {
    return (AnimType)(((enum_t)a / (enum_t)VerticalAnim::_LAST * (enum_t)LateralAnim::_LAST) % (enum_t)AnimType::_LAST);
};
Player::AnimSize Player::getAnimSize(int a) const {
    return (AnimSize)(((enum_t)a / ((enum_t)VerticalAnim::_LAST * (enum_t)LateralAnim::_LAST) * (enum_t)AnimType::_LAST) % (enum_t)AnimSize::_LAST);
};

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
    bool collisionsx = map->solveCollisionsX(posPlayer, next_pos, getSize());
    bool collisionsy = map->solveCollisionsY(posPlayer, next_pos, getSize());
    // Make the player come to a stop if there was a collision per axis
    if (collisionsx) velPlayer.x = 0.0f;
    if (collisionsy) velPlayer.y = 0.0f;
    // Apply the new position
    posPlayer = next_pos;
}

bool Player::updateYState(bool upPressed)
{
    // Get the state of the player
    bool onGround = map->onGround(posPlayer, getSize());
    bool headUnderTile = map->headUnderTile(posPlayer, getSize());
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
            bJumping = true;
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

void Player::updateAnimation(bool leftPressed, bool rightPressed) const
{
    // Overrides
    if (statePlayer == State::DYING || statePlayer == State::DEAD) {
        sprite->changeAnimation(getAnimId(SpecialAnim::DIE));
        return;
    }

    const bool onlyR = !leftPressed && rightPressed;
    const bool onlyL = leftPressed && !rightPressed;
    // Figure out components of the current animation
    int currentAnimId = sprite->animation();
    VerticalAnim verticalAnim = getVerticalAnim(currentAnimId);
    LateralAnim lateralAnim = getLateralAnim(currentAnimId);
    AnimSize animSize = getAnimSize(currentAnimId);
    AnimType animType = getAnimType(currentAnimId);
    // Figure out next vertical animation
    switch (yState) {
        case FLOOR:
            if (glm::abs(velPlayer.x) > (2.f/3.f) * X_RUN_SPEED)
                verticalAnim = VerticalAnim::SPRINT;
            else if (glm::abs(velPlayer.x) > (1.f/3.f) * X_RUN_SPEED)
                verticalAnim = VerticalAnim::RUN;
            else if (glm::abs(velPlayer.x) > X_WALK_SPEED/4.f) // FIXME: Magic number (1/4 de X_WALK_SPEED).
                verticalAnim = VerticalAnim::WALK;
            else
                verticalAnim = VerticalAnim::STAND;

            if ((velPlayer.x < 0 && onlyR) || (velPlayer.x > 0 && onlyL))
                verticalAnim = VerticalAnim::BRAKE;
            break;
        case UPWARDS:
            verticalAnim = VerticalAnim::JUMP;
            break;
        case DOWNWARDS:
            if (verticalAnim != VerticalAnim::JUMP)
                verticalAnim = VerticalAnim::STAND;
            break;
        default:
            break;
    }
    // Firgure out animation direction
    if (yState == FLOOR) {
        if (onlyL)
            lateralAnim = LateralAnim::LEFT;
        else if (onlyR)
            lateralAnim = LateralAnim::RIGHT;
    }

    // Figure out animation size
    if (statePlayer == State::BIG || statePlayer == State::BIG_STAR)
        animSize = AnimSize::BIG;
    else animSize = AnimSize::SMALL;

    // Figure out animation animation type
    if (statePlayer == State::SMALL_STAR || statePlayer == State::BIG_STAR)
        animType = AnimType::STAR;
    else animType = AnimType::NORMAL;

    // Update the animation only if it changed
    int nextAnimId = getAnimId(verticalAnim, lateralAnim, animSize, animType);
    if (nextAnimId != currentAnimId)
        sprite->changeAnimation(nextAnimId);
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

// === === State shit === ===

void Player::updatePlayerState(float deltaTime) {
    timeCurrentState += deltaTime;
    switch (statePlayer) {
        case State::SMALL:
            break;
        case State::BIG:
            break;
        case State::SMALL_STAR:
            if (timeCurrentState > 10.f) {
                setState(State::SMALL);
                timeCurrentState = 0.f;
            }
            break;
        case State::BIG_STAR:
            if (timeCurrentState > 10.f) {
                setState(State::BIG);
                timeCurrentState = 0.f;
            }
            break;
        case State::JUST_TOOK_DAMAGE:
            if (timeCurrentState > 8.f) {
                setState(State::SMALL);
                timeCurrentState = 0.f;
            }
            break;
        case State::DYING:
            if (timeCurrentState > 2.f) {
                setState(State::DEAD);
                timeCurrentState = 0.f;
            }
            break;
        case State::DEAD:
            break;
    }
}

void Player::setState(State newState) {
    if (statePlayer != newState) {
        timeCurrentState = 0.f;

        switch (statePlayer) {
            case State::SMALL:
                if (newState == State::BIG)
                    posPlayer.y -= PLAYER_BIG_SIZE.y - PLAYER_SIZE.y;
                break;
            case State::BIG:
                if (newState == State::SMALL)
                    posPlayer.y += PLAYER_BIG_SIZE.y - PLAYER_SIZE.y;
                break;
            case State::SMALL_STAR:
                if (newState == State::BIG || newState == State::BIG_STAR)
                    posPlayer.y -= PLAYER_BIG_SIZE.y - PLAYER_SIZE.y;
                break;
            case State::BIG_STAR:
                if (newState == State::SMALL || newState == State::SMALL_STAR) // Imposible
                    posPlayer.y += PLAYER_BIG_SIZE.y - PLAYER_SIZE.y;
                break;
            case State::JUST_TOOK_DAMAGE:
                if (newState == State::BIG)
                    posPlayer.y -= PLAYER_BIG_SIZE.y - PLAYER_SIZE.y;
                break;
            case State::DYING:
                break;
            case State::DEAD:
                break;
        }

        statePlayer = newState;
    }
}

void Player::stepOnEnemy() {
    this->velPlayer.y = STEP_ON_ENEMY_JUMP_VEL;
}

void Player::takeDamage() {
    if (statePlayer == State::BIG)
        setState(State::JUST_TOOK_DAMAGE);
    else
        setState(State::DYING);
}

void Player::fallDie() {
    setState(State::DEAD);
}

void Player::takeMushroom() {
    setState(State::BIG);
}

void Player::takeStar() {
    if (statePlayer == State::SMALL)
        setState(State::SMALL_STAR);
    else
     setState(State::BIG_STAR);
}
