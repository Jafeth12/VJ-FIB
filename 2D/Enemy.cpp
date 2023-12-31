#include "Enemy.h"

void Enemy::render() {
    if (!this->isDead())
        sprite->render();
}

Enemy::~Enemy() {
    if (sprite != nullptr)
        delete sprite;
}

void Enemy::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Enemy::reset() {
    bActive = false;
    vel = glm::vec2(0, 0);
}

bool Enemy::collidesWith(const Enemy &other) const {
    glm::ivec2 otherPos = other.getPosition();
    glm::ivec2 otherSize = other.getSize();

    glm::ivec2 otherMinCoords = otherPos;
    glm::ivec2 otherMaxCoords = otherPos + otherSize;

    glm::ivec2 thisMinCoords = pos;
    glm::ivec2 thisMaxCoords = pos + getSize();

    return (thisMinCoords.x < otherMaxCoords.x && thisMaxCoords.x > otherMinCoords.x &&
        thisMinCoords.y < otherMaxCoords.y && thisMaxCoords.y > otherMinCoords.y);
}
