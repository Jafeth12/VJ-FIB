#include "Scene.h"
#include "Game.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>


Scene::Scene()
{
	map = NULL;
    background = NULL;
    foreground = NULL;
    flagSprite = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
    if (background != NULL) 
        delete background;
    if (foreground != NULL) 
        delete foreground;
    if (flagSprite != NULL)
        delete flagSprite;

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
    isOver = false;
    isFinishing = false;

	if (levelFilename != "") map = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);

	currentTime = 0.0f;
}

void Scene::update(float deltaTime, Player *player)
{
	currentTime += deltaTime;
	player->update(deltaTime);

    glm::vec2 playerPos = player->getPosition();

    if (player->isOnFinishingState() && !isOver) {
        float timeToWait = 5.0f;

        if (flagSprite != NULL) {
            glm::vec2 flagPos = flagSprite->getPosition();
            glm::vec2 newPos = glm::vec2(flagPos.x, flagPos.y + 2);

            bool collision = map->onGround(newPos, glm::ivec2(32, 32));
            if (!collision) {
                flagSprite->setPosition(newPos);
            }

        }

        Player::FinishingState finishingState = player->getFinishingState();

        switch (finishingState) {
            case Player::FinishingState::POLE:
                if (!isFinishing) {
                    // play pole sound;
                }
                break;
            case Player::FinishingState::WALKING_TO_CASTLE:
                if (!isFinishing) {
                    // play end sound
                }

                break;
            case Player::FinishingState::ON_CASTLE:
                Game::instance().stopRenderingPlayer();

                static float timeAtFinishingState = currentTime;

                if (currentTime - timeAtFinishingState > timeToWait) {
                    isOver = true;
                    isFinishing = false;
                    player->setIsFinishing(false);
                    resetFlagPosition();
                }

                return;         
        }

        isFinishing = true;
    }


    hud->decrementTimeLeft();

    if (hud->isTimeLeftZero()) {
        hud->setTimeLeft(400);
    }

    if (!isFinishing) camera->setXPosition(playerPos.x - initPlayerTiles.x * map->getTileSize());
}

glm::ivec2 Scene::getInitPlayerTiles() {
    return initPlayerTiles;
}

void Scene::setBackground(std::string levelFilename) {
    if (background != NULL) delete background;
    background = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);

    glm::ivec2 poleHeadPos = background->getPoleHeadPos();
    if (poleHeadPos.x != -1) {
        flagTexture.loadFromFile("images/banderita.png", TEXTURE_PIXEL_FORMAT_RGBA);
        flagTexture.setMinFilter(GL_NEAREST);
        flagTexture.setMagFilter(GL_NEAREST);
        flagSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &flagTexture, texProgram);
        flagSprite->setPosition(glm::vec2(poleHeadPos.x - 16, poleHeadPos.y + 48));
    }
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
    if (flagSprite != NULL) flagSprite->render();

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

bool Scene::hasEnded() {
    return isOver;
}

void Scene::setIsOver(bool isOver) {
    this->isOver = isOver;
}

int Scene::getWorldNumber() {
    return worldNumber;
}

TileMap* Scene::getBackgroundMap() {
    return background;
}

TileMap* Scene::getMap() {
    return map;
}

void Scene::resetFlagPosition() {
    glm::ivec2 poleHeadPos = background->getPoleHeadPos();
    if (poleHeadPos.x != -1)
        flagSprite->setPosition(glm::vec2(poleHeadPos.x - 16, poleHeadPos.y + 48));
}
