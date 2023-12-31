#define _USE_MATH_DEFINES 
#include <cmath>
#include "Scene.h"
#include "Game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Brick.h"
#include "Interrogation.h"
#include "Scene.h"
#include "Enemy.h"
#include "Game.h"
#include "TileMap.h"

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

    for (unsigned i = 0; i < bricks.size(); ++i) delete bricks[i];
    for (unsigned i = 0; i < interrogations.size(); ++i) delete interrogations[i];
    // for (unsigned i = 0; i < interactiveBlocks.size(); ++i) delete interactiveBlocks[i];

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
    isOverworld = true;
    flagPoleIsDown = false;

	currentTime = 0.0f;
    lastSecondTime = 0.0f;
    timeAtFinishingState = 0.0f;
    lastDeathTime = 0.0f;
    scroll = 0.f;

    if (levelFilename[0] != ' ') {
        // Cargar el mapa de tiles
        map = TileMap::createTileMap(levelFilename, glm::vec2(minCoords.x, minCoords.y), *texProgram);
        if (map->getMapColor() == TileMap::MapColor::UNDERWORLD) isOverworld = false;
        initEnemies();
        initInteractiveBlocks();
        initCoins();
    }

}

void Scene::update(float deltaTime, Player *player)
{
    currentTime += deltaTime;
    player->update(deltaTime, scroll);

    for (unsigned i = 0; i < coins.size(); ++i) coins[i].update(deltaTime);
    for (unsigned i = 0; i < bricks.size(); ++i) bricks[i]->update(deltaTime);
    for (unsigned i = 0; i < interrogations.size(); ++i) interrogations[i]->update(deltaTime);

    if (player->isDead() || player->isDying()) {
        if (lastDeathTime == 0) lastDeathTime = currentTime;
        float timeToWait = 3.0f;

        if (currentTime - lastDeathTime > timeToWait) {
            isOver = true;
            initEnemies();
            lastDeathTime = 0;
        }

        return;
    }
    // for (unsigned i = 0; i < interactiveBlocks.size(); ++i) interactiveBlocks[i]->update(deltaTime);

    // Check player under the map
    if (player->getPosition().y > (map->getMapSize().y - 2) * map->getTileSize()) {
        player->fallDie();
        SoundEngine::instance().stopAllSounds();
        SoundEngine::instance().playDie();
        return;
    }

    glm::ivec2 playerPos = player->getPosition();

    if (player->isOnFinishingState() && !isOver) {
        float timeToWait = 5.0f;
        Player::FinishingState finishingState = player->getFinishingState();

        if (flagSprite != NULL) {
            glm::vec2 flagPos = flagSprite->getPosition();
            glm::vec2 newPos = glm::vec2(flagPos.x, flagPos.y + 4);

            if (finishingState == Player::FinishingState::POLE && !isFinishing) {
                glm::vec2 mapSize = map->getMapSize();

                int playerY_tile  = playerPos.y / map->getTileSize();
                int flagY_tile = flagPos.y / map->getTileSize();

                int diff = playerY_tile - flagY_tile;

                bool playerIsOnTopOfFlag = playerY_tile == flagY_tile;
                bool playerIsOnBottom = playerY_tile >= mapSize.y - 3;

                int score = 0;

                // if is on top of flag, 5000 points, if is on bottom, 400. linear interpolation between them, only multiples of 100
                if (playerIsOnTopOfFlag) {
                    score = SCORE_FLAG_TOP;
                } else if (playerIsOnBottom) {
                    score = SCORE_FLAG_BOTTOM;
                } else {
                    score = (int)(SCORE_FLAG_TOP - (SCORE_FLAG_TOP - SCORE_FLAG_BOTTOM) * (float)diff / (mapSize.y - 3));
                    score = score - score % 100;
                }

                cout << endl <<  "score: " << score << endl;
            }

            bool collision = map->onGround(newPos, glm::ivec2(32, 32));
            if (!flagPoleIsDown && collision) flagPoleIsDown = true;
            if (!flagPoleIsDown) {
                flagSprite->setPosition(newPos);
            } else {

                if (hud->getTimeLeft() > 0) {
                    Game::instance().addScore(10);
                    hud->decrementTimeLeft();
                }

                if (!hud->isTimeLeftZero() && hud->getTimeLeft()%3 == 0) SoundEngine::instance().playBeep();

                if (finishingState == Player::FinishingState::ON_CASTLE) {
                    Game::instance().stopRenderingPlayer();
                    
                    if (hud->getTimeLeft() <= 0) {
                        if (timeAtFinishingState == 0) timeAtFinishingState = currentTime;

                        if (currentTime - timeAtFinishingState > timeToWait) {
                            timeAtFinishingState = currentTime;
                            isOver = true;
                            isFinishing = false;
                            player->setIsFinishing(false);
                            resetFlagPosition();
                        }

                        return;         
                    }
                }
            }
        }


        isFinishing = true;
    }

    if (isFinishing) {
        if (player->getFinishingState() == Player::FinishingState::POLE) camera->setXPosition(playerPos.x - initPlayerTiles.x * map->getTileSize());
    } else {
        int x = playerPos.x;
        if (x >= scroll + (float)SCREEN_WIDTH/2.35) {
            scroll += abs(x - (scroll + (float)SCREEN_WIDTH/2.35));
            camera->setXPosition(scroll);
        }
    }

    // if a quarter of a second has passed, decrement time left
    if (currentTime - lastSecondTime > 0.4f) {
        lastSecondTime = currentTime;
        hud->decrementTimeLeft();
        if (hud->getTimeLeft() == 100) {
            stopTheme();
            SoundEngine::instance().playHurryUp();
        }
    }

    if (hud->isTimeLeftZero()) {
        // hay que hacer que player se peke priemro
        player->makeSmall();
        player->takeDamage();
        return;
    }

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
                    SoundEngine::instance().playKick();
                    Game::instance().addScore(SCORE_STOMP);
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
                        SoundEngine::instance().playKick();
                        Game::instance().addScore(SCORE_STOMP);
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
        if (goombas[i].shouldCollide() && player->collidesWith(goombas[i])) {
            float alpha = player->collisionAngle(goombas[i]);
            if (glm::abs(alpha) <= ANGLE_TO_DIE) {
                goombas[i].dieVertical();
                player->stepOnEnemy();
                SoundEngine::instance().playStomp();
                Game::instance().addScore(SCORE_STOMP);
            }
            else {
                if (player->isStar()) {
                    goombas[i].dieLateral();
                    SoundEngine::instance().playKick();
                    Game::instance().addScore(SCORE_STOMP);
                } else {
                    player->takeDamage();
                }
            }
        }

    // Player - koopas
    for (unsigned i = 0; i < koopas.size(); ++i) {
        if (koopas[i].shouldCollide() && player->collidesWith(koopas[i])) {
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
                    SoundEngine::instance().playStomp();
                    Game::instance().addScore(SCORE_STOMP);
                }
                else {
                    if (player->isStar()) {
                        koopas[i].dieLateral();
                        SoundEngine::instance().playKick();
                        Game::instance().addScore(SCORE_STOMP);
                    } else {
                        player->takeDamage();
                    }
                }
            }
        }
    }


    // Player - interactiveBlocks
    for (unsigned i = 0; i < bricks.size(); ++i) {
        if (player->collidesWith(*bricks[i])) {
            if (bricks[i]->canActivate()) {
                if (player->isBig()) {
                    bricks[i]->activate();
                    SoundEngine::instance().playBreakBlock();
                } else {
                    SoundEngine::instance().playBump();
                }
            }
        }
    }

    for (unsigned i = 0; i < interrogations.size(); ++i) {
        if (player->collidesWith(*interrogations[i])) {
            if (interrogations[i]->canActivate()) {
                interrogations[i]->activate();

                glm::ivec2 pos = interrogations[i]->getPos();
                Interrogation::BlockContent content = interrogations[i]->getObjectInside();

                switch (content) {
                    case Interrogation::BlockContent::COIN:
                            SoundEngine::instance().playCoin();
                            Game::instance().addScore(SCORE_COIN);
                        break;
                    case Interrogation::BlockContent::MUSHROOM:
                        for (unsigned i = 0; i < mushrooms.size(); ++i) {
                            if (mushrooms[i]->getPos() == glm::ivec2(pos.x, pos.y - map->getTileSize())) {
                                mushrooms[i]->activate();
                                SoundEngine::instance().playPowerup();
                            }
                        }
                        break;
                    case Interrogation::BlockContent::STAR:
                        for (unsigned i = 0; i < stars.size(); ++i) {
                            if (stars[i]->getPos() == glm::ivec2(pos.x, pos.y - map->getTileSize())) {
                                stars[i]->activate();
                                SoundEngine::instance().playPowerup();
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    for (unsigned i = 0; i < coins.size(); ++i) {
        if (player->collidesWith(coins[i]) && coins[i].canTake()) {
            coins[i].take();
            hud->addCoin();
            SoundEngine::instance().playCoin();
            Game::instance().addScore(SCORE_COIN);
        }
    }

    for (unsigned i = 0; i < mushrooms.size(); ++i) {
        mushrooms[i]->update(deltaTime);

        if (player->collidesWith(*mushrooms[i]) && mushrooms[i]->canBeConsumed()) {
            mushrooms[i]->consume();
            player->makeBig();
            SoundEngine::instance().playGrow();
            Game::instance().addScore(SCORE_POWERUP);
        }
    }

    for (unsigned i = 0; i < stars.size(); ++i) {
        stars[i]->update(deltaTime);

        if (player->collidesWith(*stars[i]) && stars[i]->canBeConsumed()) {
            stars[i]->consume();
            player->makeStar();
            SoundEngine::instance().playGrow();
            Game::instance().addScore(SCORE_POWERUP);
        }
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
    if (!isOverworld) glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

    for (unsigned i = 0; i < coins.size(); ++i) coins[i].render();

    for (unsigned i = 0; i < bricks.size(); ++i) {
        if (bricks[i]->shouldRender())
            bricks[i]->render();
    }
    for (unsigned i = 0; i < interrogations.size(); ++i) {
        if (interrogations[i]->shouldRender())
            interrogations[i]->render();
    }

    for (unsigned i = 0; i < mushrooms.size(); ++i) {
        if (mushrooms[i]->canBeConsumed())
            mushrooms[i]->render();
    }

    for (unsigned i = 0; i < stars.size(); ++i) {
        if (stars[i]->canBeConsumed())
            stars[i]->render();
    }

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
    if (background == NULL) return;
    if (flagSprite == NULL) return;

    glm::ivec2 poleHeadPos = background->getPoleHeadPos();
    if (poleHeadPos.x != -1)
        flagSprite->setPosition(glm::vec2(poleHeadPos.x - 16, poleHeadPos.y + 48));
}

void Scene::initEnemies() {
    goombas.clear();
    koopas.clear();

    TileMap::MapColor color = map->getMapColor();
    auto goombasPos = map->getGoombas();
    goombas.resize(goombasPos.size());
    for (unsigned i = 0; i < goombasPos.size(); ++i)
        goombas[i].init(glm::ivec2(0, 16), *texProgram, map, color, (Enemy::Dir)goombasPos[i].dir, goombasPos[i].initPos);

    auto koopasPos = map->getKoopas();
    koopas.resize(koopasPos.size());
    for (unsigned i = 0; i < koopas.size(); ++i)
        koopas[i].init(glm::ivec2(0, 16), *texProgram, map, color, (Enemy::Dir)koopasPos[i].dir, koopasPos[i].initPos);
}

void Scene::initInteractiveBlocks() {
    auto interactiveBlocksPos = map->getInteractiveBlocks();
    for (unsigned i = 0; i < interactiveBlocksPos.size(); ++i) {
        glm::ivec2 pos = interactiveBlocksPos[i].pos;
        if (interactiveBlocksPos[i].type == BRICK) {
            bricks.push_back(new Brick(glm::ivec2(0, 16), map, pos, *texProgram, map->getTexture(), map->getMapColor()));
        }
        else if (interactiveBlocksPos[i].type == INTERROGATION) {
            switch (interactiveBlocksPos[i].object) {
            case COIN:
                interrogations.push_back(new Interrogation(glm::ivec2(0, 16), map, pos, *texProgram, map->getTexture(), map->getMapColor(), Interrogation::BlockContent::COIN));
                break;
            case MUSHROOM:
                interrogations.push_back(new Interrogation(glm::ivec2(0, 16), map, pos, *texProgram, map->getTexture(), map->getMapColor(), Interrogation::BlockContent::MUSHROOM));
                mushrooms.push_back(new Mushroom(glm::ivec2(pos.x, pos.y - 1), *texProgram, map, isOverworld));
                break;
            case STAR:
                interrogations.push_back(new Interrogation(glm::ivec2(0, 16), map, pos, *texProgram, map->getTexture(), map->getMapColor(), Interrogation::BlockContent::STAR));
                stars.push_back(new Star(glm::ivec2(pos.x, pos.y - 1), *texProgram, map, isOverworld));
                break;
            default:
                break;
            }
        }
    }
}

void Scene::initCoins() {
    coins.clear();
    auto coinsPos = map->getCoins();
    coins.resize(coinsPos.size());
    for (unsigned i = 0; i < coinsPos.size(); ++i) {
        coins[i].init(*texProgram, coinsPos[i].pos, map);
    }
}

void Scene::reset() {
    isOver = false;
    isFinishing = false;
    scroll = 0;
    timeAtFinishingState = 0;
    lastDeathTime = 0;
    lastSecondTime = 0;
    flagPoleIsDown = false;
    hud->setTimeLeft(400);

    resetFlagPosition();

    for (unsigned i = 0; i < bricks.size(); ++i) bricks[i]->reset();
    for (unsigned i = 0; i < interrogations.size(); ++i) interrogations[i]->reset();
    for (unsigned i = 0; i < mushrooms.size(); ++i) mushrooms[i]->reset();
    for (unsigned i = 0; i < stars.size(); ++i) stars[i]->reset();
    for (unsigned i = 0; i < goombas.size(); ++i) goombas[i].reset();
    for (unsigned i = 0; i < koopas.size(); ++i) koopas[i].reset();

    map->remesh();
}

void Scene::playTheme() {
    if (isOverworld) {
        SoundEngine::instance().playMainTheme();
    } else {
        SoundEngine::instance().playUnderworldTheme();
    }
}

void Scene::stopTheme() {
    if (isOverworld) {
        SoundEngine::instance().stopMainTheme();
    } else {
        SoundEngine::instance().stopUnderworldTheme();
    }
}

void Scene::pauseTheme() {
    if (isOverworld) {
        SoundEngine::instance().pauseMainTheme();
    } else {
        SoundEngine::instance().pauseUnderworldTheme();
    }
}

void Scene::resumeTheme() {
    if (isOverworld) {
        SoundEngine::instance().resumeMainTheme();
    } else {
        SoundEngine::instance().resumeUnderworldTheme();
    }
}

