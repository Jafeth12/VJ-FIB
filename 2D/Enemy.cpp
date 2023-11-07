#include "Enemy.h"

void Enemy::render() {
    sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}
