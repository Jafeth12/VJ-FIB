#include "Enemy.h"

void Enemy::render() {
    sprite->render();
}

void Enemy::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

bool Enemy::collidesWith(const Enemy &other) const {
    glm::ivec2 otherPos = other.getPosition();
    glm::ivec2 otherSize = other.getSize();

    glm::ivec2 otherMinCoords = otherPos;
    glm::ivec2 otherMaxCoords = otherPos + otherSize;

    glm::ivec2 thisMinCoords = pos;
    glm::ivec2 thisMaxCoords = pos + enemySize;

    return (thisMinCoords.x < otherMaxCoords.x && thisMaxCoords.x > otherMinCoords.x &&
        thisMinCoords.y < otherMaxCoords.y && thisMaxCoords.y > otherMinCoords.y);
}
