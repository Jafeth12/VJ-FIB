#include "LoadingScene.h"
#include "Game.h"

LoadingScene::LoadingScene(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, glm::ivec2 minCoords) {
    init(shaderProgram, camera, hud, minCoords);

    finished = true;
}

LoadingScene::LoadingScene() {
}

LoadingScene::~LoadingScene() {
    // Scene::~Scene();
}

void LoadingScene::init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, glm::ivec2 minCoords) {
    initPlayerTiles = INIT_PLAYER_TILES;
    Scene::init(shaderProgram, camera, hud, "", initPlayerTiles, minCoords, -2);

    worldNumber = -1;
    lives = -1;

    texts["world"] = Text::createText("WORLD 1-" + std::to_string(worldNumber), &shaderProgram, glm::vec2(11, 14));
    texts["lives"] = Text::createText("x 3", &shaderProgram, glm::vec2(16, 18)); // TODO poner vidas aca tambien
}

void LoadingScene::update(float deltaTime, Player *player) {
    currentTime += deltaTime;
}

void LoadingScene::render() {
    glClearColor(0, 0, 0, 1);

    if (currentTime < LOADING_SCREEN_TIME) {
        Scene::render();
    } else {
        finished = true;
    }
}

void LoadingScene::start() {
    finished = false;
    currentTime = 0.f;
}

void LoadingScene::reset() {
    finished = true;
    currentTime = 0.f;
}

bool LoadingScene::isFinished() {
    return finished;
}

void LoadingScene::setWorldNumber(int worldNumber) {
    this->worldNumber = worldNumber;
    texts["world"]->updateText("WORLD 1-" + std::to_string(worldNumber));
}

void LoadingScene::setLives(int lives) {
    this->lives = lives;
}
