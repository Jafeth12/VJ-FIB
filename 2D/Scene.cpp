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

    time_left = 400;

    texts.push_back(Text::createText("Mario", &shaderProgram, glm::vec2(48, 16)));
    texts.push_back(Text::createText("000000", &shaderProgram, glm::vec2(48, 32)));
    texts.push_back(Text::createText("0x00", &shaderProgram, glm::vec2(208, 32)));
    texts.push_back(Text::createText("World", &shaderProgram, glm::vec2(384, 16)));
    texts.push_back(Text::createText("1-1", &shaderProgram, glm::vec2(400, 32)));
    texts.push_back(Text::createText("Time", &shaderProgram, glm::vec2(496, 16)));
    texts.push_back(Text::createText(std::to_string(time_left), &shaderProgram, glm::vec2(512, 32)));
	currentTime = 0.0f;
}

void Scene::update(int deltaTime, Player *player)
{
	currentTime += deltaTime;
	player->update(deltaTime);

    --time_left;
    texts[6]->updateText(std::to_string(time_left));

    glm::vec2 playerPos = player->getPosition();
    camera->setXPosition(playerPos.x - INIT_PLAYER_X_TILES * map->getTileSize());
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

	map->render();

    render_texts();
	texProgram->setUniformMatrix4f("view", view);
}

void Scene::render_texts() {
    for (size_t i = 0; i < texts.size(); ++i) {
        texts[i]->render();
    }
}

TileMap* Scene::getMap() {
    return map;
}
