#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <map>
#include "Camera.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"
#include "HUD.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	virtual void init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords);
	virtual void update(float deltaTime, Player *player);
	virtual void render();
    TileMap* getMap();

    void setBackground(std::string levelFilename);
    void setForeground(std::string levelFilename);

protected:

	ShaderProgram *texProgram;

	TileMap *map;
    TileMap *background;
    TileMap *foreground;

    HUD *hud;

    glm::ivec2 initPlayerTiles;
    glm::ivec2 minCoords;

    std::map<std::string, Text*> texts;

    Camera *camera;

	float currentTime;

private:

};


#endif // _SCENE_INCLUDE

