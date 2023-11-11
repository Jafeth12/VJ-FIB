#ifndef POWERUP_H
#define POWERUP_H

#include "Sprite.h"
#include "Texture.h"
#include "TileMap.h"
#include "Constants.h"

#define POWERUP_SPEED 120.f
#define POWERUP_SIZE glm::vec2(32, 32)

class Powerup {

public:
    Powerup();
    virtual ~Powerup();

    virtual void init(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map);
    virtual void update(float deltaTime);
    virtual void render();

    void setPosition(const glm::vec2 &pos);
    void setTileMap(TileMap *tileMap);
    void setSprite(Texture *spritesheet);
    void invertDirection() { dir = (Dir)(-(char)dir); };

    void activate();

    glm::ivec2 getSize() const { return POWERUP_SIZE; };
    glm::ivec2 getPos() const;
    glm::vec2 getVel() const;

    bool canBeConsumed() const { return active && !consumed; };
    bool isConsumed() const;

    void consume();

protected:
    enum class Dir : char { LEFT=-1, RIGHT=1 };

    Texture texture;
    Sprite *sprite;
    TileMap *map;

    ShaderProgram *shaderProgram;

    glm::ivec2 pos;
    glm::vec2 vel;
    Dir dir;

    bool active;
    bool consumed;

    void updateVelocity(float deltaTime);
    void updatePosition(float deltaTime);
};

#endif // !POWERUP_H
