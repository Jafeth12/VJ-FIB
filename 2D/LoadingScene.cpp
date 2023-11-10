#include "LoadingScene.h"
#include "Game.h"

#define PLAYER_SIZE_IN_SPRITESHEET 16.f
#define PLAYER_SIZE 32
#define TILE_SIZE 32

LoadingScene::LoadingScene(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, glm::ivec2 minCoords) {
    marioSprite = NULL;

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

    marioSpritesheet.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
    marioSpritesheet.setMinFilter(GL_NEAREST);
    marioSpritesheet.setMagFilter(GL_NEAREST);

    float size = PLAYER_SIZE_IN_SPRITESHEET / marioSpritesheet.width();
    marioSprite = Sprite::createSprite(glm::vec2(PLAYER_SIZE), glm::vec2(size, size), &marioSpritesheet, &shaderProgram);
    marioSprite->setNumberAnimations(1);
    marioSprite->setAnimationSpeed(0, 8);
    marioSprite->addKeyframe(0, glm::vec2(0.f, 1.f));
    marioSprite->changeAnimation(0);

    glm::vec2 initPlayerPos = glm::vec2(initPlayerTiles.x * TILE_SIZE, (initPlayerTiles.y * TILE_SIZE));

    marioSprite->setPosition(glm::vec2(initPlayerPos.x, initPlayerPos.y - 16));

    texts["world"] = Text::createText("WORLD 1-" + std::to_string(worldNumber), &shaderProgram, glm::vec2(11, 14));
    texts["lives"] = Text::createText("x " + std::to_string(lives), &shaderProgram, glm::vec2(16, 18)); // TODO poner vidas aca tambien
}

void LoadingScene::update(float deltaTime, Player *player) {
    currentTime += deltaTime;
}

void LoadingScene::render() {
    glClearColor(0, 0, 0, 1);

    if (currentTime < LOADING_SCREEN_TIME) {
        marioSprite->render();
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
    texts["lives"]->updateText("x " + std::to_string(lives));
}
