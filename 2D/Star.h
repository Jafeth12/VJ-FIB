#ifndef STAR_H
#define STAR_H

#include "Powerup.h"

class Star : public Powerup {

public:
    Star();
    Star(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld);

    void init(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld);

private:
};

#endif // !STAR_H
