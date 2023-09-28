#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "Camera.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram &shaderProgram, Camera &camera, std::string levelFilename);
	void update(float deltaTime, Player *player);
	void render();
    TileMap* getMap();

private:
	TileMap *map;
	ShaderProgram *texProgram;
	float currentTime;
    Camera *camera;
};


#endif // _SCENE_INCLUDE

