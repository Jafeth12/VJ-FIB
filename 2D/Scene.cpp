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

    timeLeft = 400;

    texts["mario"] = Text::createText("Mario", &shaderProgram, glm::vec2(3, 1));
    texts["score"] = Text::createText("000000", &shaderProgram, glm::vec2(3, 2));
    texts["coins"] = Text::createText("0x00", &shaderProgram, glm::vec2(13, 2));
    texts["worldText"] = Text::createText("World", &shaderProgram, glm::vec2(24, 1));
    texts["worldNumber"] = Text::createText("1-1", &shaderProgram, glm::vec2(25, 2));
    texts["timeText"] = Text::createText("Time", &shaderProgram, glm::vec2(31, 1));
    texts["timeNumber"] = Text::createText(std::to_string(timeLeft), &shaderProgram, glm::vec2(32, 2));
	currentTime = 0.0f;
}

void Scene::update(float deltaTime, Player *player)
{
	currentTime += deltaTime;
	player->update(deltaTime);

    // TODO quitar todo esto, es de prueba

    --timeLeft;

    if (timeLeft == 0) {
        timeLeft = 400;
    }

    if (timeLeft <= 100)
        texts["timeNumber"]->updateText("0" + std::to_string(timeLeft));
    else if (timeLeft <= 10)
        texts["timeNumber"]->updateText("00" + std::to_string(timeLeft));
    else
        texts["timeNumber"]->updateText(std::to_string(timeLeft));

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

	texProgram->setUniformMatrix4f("view", view);   // esto está aquí porque el render de player necesita la view matrix de la cámara
                                                    // el player se renderiza justo después de esto. habría que mirárselo.
}

void Scene::render_texts() {
    for (auto it = texts.begin(); it != texts.end(); ++it) {
        it->second->render();
    }
}

TileMap* Scene::getMap() {
    return map;
}
