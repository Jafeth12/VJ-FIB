#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "Powerup.h"

class Mushroom : public Powerup {

public:
    Mushroom();
    Mushroom(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld);

    void init(glm::ivec2 tileMapPos, ShaderProgram &shaderProgram, TileMap *map, bool overworld);

private:

};


#endif // !MUSHROOM_H
