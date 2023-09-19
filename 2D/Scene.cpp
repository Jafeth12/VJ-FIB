#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init(ShaderProgram &shaderProgram, Camera &cam, std::string levelFilename)
{
    texProgram = &shaderProgram;
	// initShaders();
	map = TileMap::createTileMap(levelFilename, glm::vec2(SCREEN_X, SCREEN_Y), *texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), *texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
    camera = &cam;
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
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
	player->render();
}
