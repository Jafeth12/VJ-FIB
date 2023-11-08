#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "Camera.h"
#include "Koopa.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"
#include "Goomba.h"
#include "HUD.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	virtual void init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords, int worldNumber);
	virtual void update(float deltaTime, Player *player);
	virtual void render();
    TileMap* getMap();

    void setBackground(std::string levelFilename);
    void setForeground(std::string levelFilename);

    int getWorldNumber();

    glm::ivec2 getInitPlayerTiles();

protected:

	ShaderProgram *texProgram;

	TileMap *map;
    TileMap *background;
    TileMap *foreground;

    HUD *hud;

    std::vector<Goomba> goombas;
    Koopa koopa;

    glm::ivec2 initPlayerTiles;
    glm::ivec2 minCoords;

    std::map<std::string, Text*> texts;
    bool autoRenderAllText;

    Camera *camera;

	float currentTime;
    int worldNumber;

private:
};


#endif // _SCENE_INCLUDE

