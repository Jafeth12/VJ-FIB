#include "Scene.h"
#include "Enemy.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>


Scene::Scene()
{
	map = NULL;
    background = NULL;
    foreground = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
    if (background != NULL) 
        delete background;
    if (foreground != NULL) 
        delete foreground;

    for (auto &text : texts) {
        delete text.second;
    }
}

void Scene::init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords, int worldNumber)
{
    texProgram = &shaderProgram;

    this->initPlayerTiles = initPlayerTiles;
    this->minCoords = minCoords;
    this->hud = &hud;
    this->camera = &camera;
    this->worldNumber = worldNumber;

    autoRenderAllText = true;

	if (levelFilename[0] != ' ')
        map = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);

	currentTime = 0.0f;

    Enemy::Color color = Enemy::Color::OVERWORLD;

    // TODO esto tampoco debe estar hardcodeado XD, pero esk los defines estan en Game.cpp
    goombas.resize(2);
    for (unsigned i = 0; i < goombas.size(); ++i)
        goombas[i].init(glm::ivec2(0, 16), shaderProgram, map, color, Enemy::Dir::RIGHT, glm::ivec2(3, 13));

    koopas.resize(2);
    for (unsigned i = 0; i < koopas.size(); ++i)
        koopas[i].init(glm::ivec2(0, 16), shaderProgram, map, color, Enemy::Dir::RIGHT, glm::ivec2(5, 12));
}

void Scene::update(float deltaTime, Player *player)
{
	currentTime += deltaTime;
	player->update(deltaTime);
    for (unsigned i = 0; i < goombas.size(); ++i) goombas[i].update(deltaTime);
    for (unsigned i = 0; i < koopas.size(); ++i) koopas[i].update(deltaTime);

    hud->decrementTimeLeft();

    if (hud->isTimeLeftZero()) {
        hud->setTimeLeft(400);
    }

    glm::vec2 playerPos = player->getPosition();
    camera->setXPosition(playerPos.x - initPlayerTiles.x * map->getTileSize());
}

glm::ivec2 Scene::getInitPlayerTiles() {
    return initPlayerTiles;
}

void Scene::setBackground(std::string levelFilename) {
    if (background != NULL) delete background;
    background = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);
}

void Scene::setForeground(std::string levelFilename) {
    if (foreground != NULL) delete foreground;
    foreground = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);
}

void Scene::render() {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();

	texProgram->use();
	texProgram->setUniformMatrix4f("projection", projection);
	texProgram->setUniformMatrix4f("model", model);
	texProgram->setUniformMatrix4f("view", view);
	texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

    if (background != NULL) background->render();
	if (map != NULL) map->render();
    if (foreground != NULL) foreground->render();

    for (unsigned i = 0; i < goombas.size(); ++i) goombas[i].render();
    for (unsigned i = 0; i < koopas.size(); ++i) koopas[i].render();

    if (worldNumber > 0) hud->setWorldNumber(worldNumber);
    hud->render();

    if (autoRenderAllText) {
        for (auto it = texts.begin(); it != texts.end(); ++it) {
            it->second->render();
        }
    }

	texProgram->setUniformMatrix4f("view", view);   // esto está aquí porque el render de player necesita la view matrix de la cámara
                                                    // el player se renderiza justo después de esto. habría que mirárselo.
}

int Scene::getWorldNumber() {
    return worldNumber;
}

TileMap* Scene::getMap() {
    return map;
}
