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


void Scene::init(ShaderProgram &shaderProgram)
{
    texProgram = &shaderProgram;
	// initShaders();
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), *texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), *texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
}

void Scene::render(Camera& camera) {
    // mario horizontal scrolling effect 
    glm::vec2 playerPos = player->getPosition();

    // camera follows player
    camera.setXPosition(playerPos.x - INIT_PLAYER_X_TILES * map->getTileSize());

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

	texProgram->use();
	texProgram->setUniformMatrix4f("model", model);
	texProgram->setUniformMatrix4f("view", view);
	texProgram->setUniformMatrix4f("projection", projection);
	texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	player->render();
}

void Scene::initShaders()
{
	// Shader vShader, fShader;
	//
	// vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	// if(!vShader.isCompiled())
	// {
	// 	cout << "Vertex Shader Error" << endl;
	// 	cout << "" << vShader.log() << endl << endl;
	// }
	// fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	// if(!fShader.isCompiled())
	// {
	// 	cout << "Fragment Shader Error" << endl;
	// 	cout << "" << fShader.log() << endl << endl;
	// }
	// texProgram.init();
	// texProgram.addShader(vShader);
	// texProgram.addShader(fShader);
	// texProgram.link();
	// if(!texProgram.isLinked())
	// {
	// 	cout << "Shader Linking Error" << endl;
	// 	cout << "" << texProgram.log() << endl << endl;
	// }
	// texProgram.bindFragmentOutput("outColor");
	// vShader.free();
	// fShader.free();
}



