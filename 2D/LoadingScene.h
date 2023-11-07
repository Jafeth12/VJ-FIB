#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H

#include "Scene.h"

#define LOADING_SCREEN_TIME 3.f

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
    constexpr static glm::ivec2 INIT_PLAYER_TILES = glm::ivec2(6, 8);

    int worldNumber;
    int lives;

    bool finished;
};

#endif // !LOADING_SCENE_H
