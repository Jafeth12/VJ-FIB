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

	currentTime = 0.0f;

    if (levelFilename[0] != ' ') {
        // Cargar el mapa de tiles
        map = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);

        // Cargar los enemigos del mapa
        // Not my proudest fap
        auto goombasPos = map->getGoombas();
        goombas.resize(goombasPos.size());
        for (unsigned i = 0; i < goombasPos.size(); ++i) {
            Enemy::Dir d;
            if (goombasPos[i].dir == 'R') d = Enemy::Dir::RIGHT;
            else d = Enemy::Dir::LEFT;

            Enemy::Color color;
            if (goombasPos[i].color == 'U') color = Enemy::Color::UNDERWORLD;
            else color = Enemy::Color::OVERWORLD;

            goombas[i].init(glm::ivec2(0, 16), shaderProgram, map, color, d, goombasPos[i].initPos);
        }

        auto koopasPos = map->getKoopas();
        koopas.resize(koopasPos.size());
        for (unsigned i = 0; i < koopas.size(); ++i) {
            Enemy::Dir d;
            if (koopasPos[i].dir == 'R') d = Enemy::Dir::RIGHT;
            else d = Enemy::Dir::LEFT;

            Enemy::Color color;
            if (goombasPos[i].color == 'U') color = Enemy::Color::UNDERWORLD;
            else color = Enemy::Color::OVERWORLD;

            koopas[i].init(glm::ivec2(0, 16), shaderProgram, map, color, d, koopasPos[i].initPos);
        }
    }

}

void Scene::update(float deltaTime, Player *player)
{
    currentTime += deltaTime;
    player->update(deltaTime);

    // Goombas
    for (unsigned i = 0; i < goombas.size(); ++i) {
        // Activate when player is near
        if (!goombas[i].isActive() && abs(goombas[i].getPosition().x - player->getPosition().x) < 500)
            goombas[i].activate();
        if (!goombas[i].isDead()) {
            // Update conditionally (if not dead)
            goombas[i].update(deltaTime);
            // Check if under the map. Die if so
            if (goombas[i].getPosition().y > (map->getMapSize().y - 2) * map->getTileSize()) {
                goombas[i].dieFall();
            }
        }
    }
    // Koopas
    for (unsigned i = 0; i < koopas.size(); ++i) {
        // Activate when player is near
        if (!koopas[i].isActive() && abs(koopas[i].getPosition().x - player->getPosition().x) < 500)
            koopas[i].activate();
        if (!koopas[i].isDead()) {
            // Update conditionally (if not dead)
            koopas[i].update(deltaTime);
            // Check if under the map. Die if so
            if (koopas[i].getPosition().y > (map->getMapSize().y - 2) * map->getTileSize())
                koopas[i].dieFall();
        }
    }

    // Check collisions
    // Goombas - Goombas
    for (unsigned i = 0; i < goombas.size(); ++i)
        for (unsigned j = 0; j < i; ++j)
            if (goombas[i].shouldCollide() && goombas[j].shouldCollide() && goombas[i].collidesWith(goombas[j])) {
                goombas[i].invertDirection();
                goombas[j].invertDirection();
            }

    // Goombas - Koopas
    for (unsigned i = 0; i < goombas.size(); ++i)
        for (unsigned j = 0; j < koopas.size(); ++j)
            if (goombas[i].shouldCollide() && koopas[j].shouldCollide() && goombas[i].collidesWith(koopas[j])) {
                if (koopas[j].isMovingShell()) {
                    goombas[i].dieLateral();
                } else {
                    goombas[i].invertDirection();
                    koopas[j].invertDirection();
                }
            }

    // Koopas Koopas
    for (unsigned i = 0; i < koopas.size(); ++i)
        for (unsigned j = 0; j < i; ++j)
            if (koopas[i].shouldCollide() && koopas[j].shouldCollide() && koopas[i].collidesWith(koopas[j])) {
                if (koopas[i].isMovingShell() ^ koopas[j].isMovingShell()) {
                    if (koopas[i].isMovingShell()) {
                        koopas[j].dieLateral();
                    } else {
                        koopas[i].dieLateral();
                    }
                } else {
                    koopas[i].invertDirection();
                    koopas[j].invertDirection();
                }
            }

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

    for (unsigned i = 0; i < goombas.size(); ++i) 
        if (!goombas[i].isDead())
                goombas[i].render();
    for (unsigned i = 0; i < koopas.size(); ++i)
        if (!koopas[i].isDead())
                koopas[i].render();

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
