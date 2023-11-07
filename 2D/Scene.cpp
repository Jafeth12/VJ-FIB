#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init(ShaderProgram &shaderProgram, Camera &cam, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords)
{
    texProgram = &shaderProgram;

    this->initPlayerTiles = initPlayerTiles;
    this->minCoords = minCoords;

	map = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);

    // TODO quitar esto hardcodeado porque cambia por escena (ponerlo en la creadora?)
    background = TileMap::createTileMap("levels/background01.txt", glm::vec2(minCoords.x, minCoords.y), *texProgram);
    camera = &cam;

    timeLeft = 400;

    // TODO esto too no debe ser hardcodeado porque puede cambiar (scene deberia tener score, coins, etc)
    texts["mario"] = Text::createText("Mario", &shaderProgram, glm::vec2(3, 1));
    texts["score"] = Text::createText("000000", &shaderProgram, glm::vec2(3, 2));
    texts["coins"] = Text::createText("0x00", &shaderProgram, glm::vec2(12, 2));
    texts["worldText"] = Text::createText("World", &shaderProgram, glm::vec2(19, 1));
    texts["worldNumber"] = Text::createText("1-1", &shaderProgram, glm::vec2(20, 2));
    texts["timeText"] = Text::createText("Time", &shaderProgram, glm::vec2(25, 1));
    texts["timeNumber"] = Text::createText(std::to_string(timeLeft), &shaderProgram, glm::vec2(26, 2));
	currentTime = 0.0f;

    // TODO esto tampoco debe estar hardcodeado XD, pero esk los defines estan en Game.cpp
    goomba.init(glm::ivec2(0, 16), shaderProgram, map, Enemy::Color::OVERWORLD, Enemy::Dir::RIGHT, glm::ivec2(4, 13));
}

void Scene::update(float deltaTime, Player *player)
{
	currentTime += deltaTime;
	player->update(deltaTime);
    goomba.update(deltaTime);

    // TODO quitar todo esto, es de prueba

    --timeLeft;

    if (timeLeft == 0) timeLeft = 400;

    if (timeLeft < 10)
        texts["timeNumber"]->updateText("00" + std::to_string(timeLeft));
    else if (timeLeft < 100)
        texts["timeNumber"]->updateText("0" + std::to_string(timeLeft));
    else
        texts["timeNumber"]->updateText(std::to_string(timeLeft));

    glm::vec2 playerPos = player->getPosition();
    camera->setXPosition(playerPos.x - initPlayerTiles.x * map->getTileSize());
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

    background->render();
	map->render();
    goomba.render();

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
