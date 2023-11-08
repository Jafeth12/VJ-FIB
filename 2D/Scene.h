#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <map>
#include "Camera.h"
#include "Koopa.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"
#include "Goomba.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram &shaderProgram, Camera &camera, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords);
	void update(float deltaTime, Player *player);
	void render();
    TileMap* getMap();

private:
	TileMap *map;
    TileMap *background;
	ShaderProgram *texProgram;
	float currentTime;

    Goomba goomba;
    Koopa koopa;

    glm::ivec2 initPlayerTiles;
    glm::ivec2 minCoords;

    std::map<std::string, Text*> texts;

    Camera *camera;
    int timeLeft;

    void render_texts();
};


#endif // _SCENE_INCLUDE

