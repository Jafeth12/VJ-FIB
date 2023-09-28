#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init(ShaderProgram &shaderProgram, Camera &cam, std::string levelFilename)
{
    texProgram = &shaderProgram;
	// initShaders();
	map = TileMap::createTileMap(levelFilename, glm::vec2(SCREEN_X, SCREEN_Y), *texProgram);
    camera = &cam;
	currentTime = 0.0f;
}

void Scene::update(float deltaTime, Player *player)
{
	currentTime += deltaTime;
	player->update(deltaTime);

    glm::vec2 playerPos = player->getPosition();
    camera->setXPosition(playerPos.x - INIT_PLAYER_X_TILES * map->getTileSize());
}

void Scene::render() {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();

	texProgram->use();
	texProgram->setUniformMatrix4f("model", model);
	texProgram->setUniformMatrix4f("view", view);
	texProgram->setUniformMatrix4f("projection", projection);
	texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
}

TileMap* Scene::getMap() {
    return map;
}
