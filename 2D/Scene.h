#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "Brick.h"
#include "Camera.h"
#include "Interrogation.h"
#include "Koopa.h"
#include "Mushroom.h"
#include "ShaderProgram.h"
#include "Star.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"
#include "Goomba.h"
#include "HUD.h"
#include "InteractiveBlock.h"

#include "Coin.h"


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
    TileMap* getBackgroundMap();

    void setBackground(std::string levelFilename);
    void setForeground(std::string levelFilename);

    bool hasEnded();
    void setIsOver(bool isOver);

    void playTheme();
    void stopTheme();

    void pauseTheme();
    void resumeTheme();

    void reset();

    int getWorldNumber();

    glm::ivec2 getInitPlayerTiles();

    bool isFinishingScene() { return isFinishing; }

protected:

	ShaderProgram *texProgram;

	TileMap *map;
    TileMap *background;
    TileMap *foreground;

    Sprite *flagSprite;
    Texture flagTexture;

    HUD *hud;

    std::vector<Goomba> goombas;
    std::vector<Koopa> koopas;

    // std::vector<InteractiveBlock*> interactiveBlocks;
    std::vector<Brick*> bricks;
    std::vector<Interrogation*> interrogations;
    std::vector<Mushroom*> mushrooms;
    std::vector<Star*> stars;

    std::vector<Coin> coins;

    glm::ivec2 initPlayerTiles;
    glm::ivec2 minCoords;

    std::map<std::string, Text*> texts;
    bool autoRenderAllText;

    Camera *camera;

	float currentTime;
    float lastSecondTime;
    float timeAtFinishingState;
    float lastDeathTime;
    int worldNumber;

    bool isOver;
    bool isFinishing;

    bool isOverworld;
    bool flagPoleIsDown;

    float scroll;

private:
    void resetFlagPosition();
    void initEnemies();
    void initInteractiveBlocks();
    void initCoins();
};


#endif // _SCENE_INCLUDE

