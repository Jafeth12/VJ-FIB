#include "Scene.h"
#include "Game.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Enemy.h"

#define DISTANCE_TO_ACTIVATE_ENEMY 500.f
#define ANGLE_TO_DIE (M_PI/2.f)

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

	currentTime = 0.0f;
    lastSecondTime = 0.0f;

    if (levelFilename[0] != ' ') {
        // Cargar el mapa de tiles
        map = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);

        initEnemies(shaderProgram);
    }

}

void Scene::update(float deltaTime, Player *player)
{
    currentTime += deltaTime;
    player->update(deltaTime);

    if (player->isDead() || player->isDying()) {
        static float timeAtDeath = currentTime;
        if (timeAtDeath == 0) timeAtDeath = currentTime;
        float timeToWait = 3.0f;

        if (currentTime - timeAtDeath > timeToWait) {
            isOver = true;
            initEnemies(*texProgram);
            timeAtDeath = 0;
        }

        return;
    }

    // Check player under the map
    if (player->getPosition().y > (map->getMapSize().y - 2) * map->getTileSize()) {
        player->fallDie();
        return;
    }

    glm::ivec2 playerPos = player->getPosition();

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

    if (!isFinishing) camera->setXPosition(playerPos.x - initPlayerTiles.x * map->getTileSize());

    // === Conditionally update enemies ===
    // Goombas
    for (unsigned i = 0; i < goombas.size(); ++i) {
        // Activate when player is near
        if (!goombas[i].isActive() && abs(goombas[i].getPosition().x - player->getPosition().x) < DISTANCE_TO_ACTIVATE_ENEMY)
            goombas[i].activate();
        if (!goombas[i].isDead()) {
            // Update conditionally (if not dead)
            goombas[i].update(deltaTime);
            // Check if under the map. Die if so
                if (goombas[i].getPosition().y > (map->getMapSize().y - 1) * map->getTileSize()) {
                goombas[i].dieFall();
            }
        }
    }
    // Koopas
    for (unsigned i = 0; i < koopas.size(); ++i) {
        // Activate when player is near
        if (!koopas[i].isActive() && abs(koopas[i].getPosition().x - player->getPosition().x) < DISTANCE_TO_ACTIVATE_ENEMY)
            koopas[i].activate();
        if (!koopas[i].isDead()) {
            // Update conditionally (if not dead)
            koopas[i].update(deltaTime, player->getPosition());
            // Check if under the map. Die if so
            if (koopas[i].getPosition().y > (map->getMapSize().y - 1) * map->getTileSize())
                koopas[i].dieFall();
            if (koopas[i].isShell() && !camera->isOnScreen(koopas[i].getPosition(), koopas[i].getSize())) {
                koopas[i].dieFall();
            }
        }
    }

    // === Check collisions ===
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

    // Player - goombas
    for (unsigned i = 0; i < goombas.size(); ++i)
        if (goombas[i].shouldCollide() && player->collidesWithEnemy(goombas[i])) {
            float alpha = player->collisionAngle(goombas[i]);
            if (glm::abs(alpha) <= ANGLE_TO_DIE) {
                goombas[i].dieVertical();
                player->stepOnEnemy();
            }
            else {
                player->takeDamage();
            }
        }

    // Player - koopas
    for (unsigned i = 0; i < koopas.size(); ++i) {
        if (koopas[i].shouldCollide() && player->collidesWithEnemy(koopas[i])) {
            if (koopas[i].isShell() && !koopas[i].isMovingShell()) {
                koopas[i].kick(koopas[i].kickDirection(*player));
            }
            else if (koopas[i].isMovingShell()) {
                player->takeDamage();
            }
            else {
                float alpha = player->collisionAngle(koopas[i]);
                if (glm::abs(alpha) <= ANGLE_TO_DIE) {
                    koopas[i].dieVertical();
                    player->stepOnEnemy();
                }
                else {
                    player->takeDamage();
                }
            }
        }
    }

    // if a second has passed, decrement time left
    if (currentTime - lastSecondTime > 1.0f) {
        lastSecondTime = currentTime;
        hud->decrementTimeLeft();
    }

    if (hud->isTimeLeftZero()) {
        player->takeDamage();
    }
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

void Scene::initEnemies(ShaderProgram &program) {
        TileMap::MapColor color = map->getMapColor();
        auto goombasPos = map->getGoombas();
        goombas.resize(goombasPos.size());
        for (unsigned i = 0; i < goombasPos.size(); ++i)
            goombas[i].init(glm::ivec2(0, 16), program, map, color, (Enemy::Dir)goombasPos[i].dir, goombasPos[i].initPos);

        auto koopasPos = map->getKoopas();
        koopas.resize(koopasPos.size());
        for (unsigned i = 0; i < koopas.size(); ++i)
            koopas[i].init(glm::ivec2(0, 16), program, map, color, (Enemy::Dir)koopasPos[i].dir, koopasPos[i].initPos);

}
