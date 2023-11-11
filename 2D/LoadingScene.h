#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H

#include "Scene.h"

#define LOADING_SCREEN_TIME 1.5f
#define INIT_PLAYER_TILES glm::ivec2(6, 9)

class LoadingScene : public Scene {
public:
	LoadingScene(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, glm::ivec2 minCoords);
    LoadingScene();
    ~LoadingScene();

	void init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, glm::ivec2 minCoords);
    void update(float deltaTime, Player *player);
    void render();

    void setWorldNumber(int worldNumber);

    void setLives(int lives);
    bool isFinished();

    void start();
    void reset();

private:
    Sprite *marioSprite;
    Texture marioSpritesheet;

    int worldNumber;
    int lives;

    bool finished;
};

#endif // !LOADING_SCENE_H
