#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 16

#define SCENE_0_INIT_PLAYER_TILES glm::ivec2(3, 13)
#define SCENE_1_INIT_PLAYER_TILES glm::ivec2(3, 13)

Game::~Game() {}

void Game::init()
{
    currentState = GAME_MENU;
    currentSceneIndex = 0;
    isRenderingPlayer = true;
	bPlay = true;
    wireframe = false;
    showsLoadingScene = true;
	glClearColor(0.45, 0.45f, 1.0f, 1.0f);
    camera = Camera(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    initShaders();
    Text::init();
    SoundEngine::instance().init();

    hud.init(shaderProgram);

	menu.init(shaderProgram, camera, hud, "levels/level01.txt", SCENE_0_INIT_PLAYER_TILES, glm::ivec2(SCREEN_X, SCREEN_Y));
    menu.setBackground("levels/background01.txt");

    loadingScene.init(shaderProgram, camera, hud, glm::ivec2(SCREEN_X, SCREEN_Y));

    // create scenes
    scenes.push_back(new Scene());
    scenes.push_back(new Scene());
	scenes[currentSceneIndex]->init(shaderProgram, camera, hud, "levels/level01.txt", SCENE_0_INIT_PLAYER_TILES, glm::ivec2(SCREEN_X, SCREEN_Y), 1);
    scenes[currentSceneIndex]->setBackground("levels/background01.txt");

	// scenes[currentSceneIndex+1]->init(shaderProgram, camera, hud, "levels/level02.txt", SCENE_1_INIT_PLAYER_TILES, glm::ivec2(SCREEN_X, SCREEN_Y), 2);
	scenes[currentSceneIndex+1]->init(shaderProgram, camera, hud, "levels/level01.txt", SCENE_1_INIT_PLAYER_TILES, glm::ivec2(SCREEN_X, SCREEN_Y), 2);
    scenes[currentSceneIndex+1]->setBackground("levels/background01.txt");

    TileMap *map = scenes[currentSceneIndex]->getMap();
    TileMap *backgroundMap = scenes[currentSceneIndex]->getBackgroundMap();

    player = new Player();

    glm::ivec2 initPlayerTiles = menu.getInitPlayerTiles();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderProgram);
	player->setPosition(glm::vec2(initPlayerTiles.x * map->getTileSize(), initPlayerTiles.y * map->getTileSize()));
	player->setTileMap(map);
    player->setBackgroundMap(backgroundMap);

    // player->moveTo(glm::vec2(500, 0));
}

bool Game::update(float deltaTime)
{

    TileMap *newTileMap = scenes[currentSceneIndex]->getMap();
    TileMap *newBackgroundMap = scenes[currentSceneIndex]->getBackgroundMap();

    switch (currentState) {
        case GAME_MENU:
            SoundEngine::instance().stopAllSounds();
            player->setPosition(glm::vec2(menu.getInitPlayerTiles().x * newTileMap->getTileSize(), menu.getInitPlayerTiles().y * newTileMap->getTileSize()));
            camera.setPosition(glm::vec2(0, 0));
            menu.update(deltaTime);
            if (menu.getMenuState() == MainMenu::MenuState::PLAY) {
                currentState = GAME_PLAY;
                hud.showTimeLeft();
                changeScene(0);
            }
            break;
        case GAME_LOADING:
            if (loadingScene.isFinished()) {
                startRenderingPlayer();
                hud.showTimeLeft();
                glm::ivec2 initPlayerTiles = scenes[currentSceneIndex]->getInitPlayerTiles();
                player->setPosition(glm::vec2(initPlayerTiles.x * newTileMap->getTileSize(), initPlayerTiles.y * newTileMap->getTileSize()));
                player->setTileMap(newTileMap);
                SoundEngine::instance().playMainTheme();
            } else {
                SoundEngine::instance().stopAllSounds();
                // player->setPosition(glm::vec2(loadingScene.getInitPlayerTiles().x * newTileMap->getTileSize(), loadingScene.getInitPlayerTiles().y * newTileMap->getTileSize()));
                camera.setPosition(glm::vec2(0, 0));
            }

            loadingScene.update(deltaTime, player);
            break;
        case GAME_PLAY:
            scenes[currentSceneIndex]->update(deltaTime, player);

            if (scenes[currentSceneIndex]->hasEnded()) {
                scenes[currentSceneIndex]->setIsOver(false);
                startRenderingPlayer();

                if (player->isDead() || player->isDying()) {
                    player->makeAlive();
                    changeScene(currentSceneIndex);
                    break;
                }

                int size = scenes.size();
                if (currentSceneIndex + 1 < size) {
                    changeScene(currentSceneIndex + 1);
                } else {
                    currentState = GAME_MENU;
                    menu.setMenuState(MainMenu::MenuState::TITLE);
                    // falta el poner top score y todo eso
                    hud.hideTimeLeft();
                }
            }

            break;
    }

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.45, 0.45f, 1.0f, 1.0f);

    switch (currentState) {
        case GAME_MENU:
            menu.render();
            break;
        case GAME_LOADING:

            if (loadingScene.isFinished()) {
                currentState = GAME_PLAY;
            } else {
                hud.hideTimeLeft();
                loadingScene.render();
            }

            break;
        case GAME_PLAY:
            scenes[currentSceneIndex]->render();
            break;
    }

    if (isRenderingPlayer) player->render();
}

void Game::keyPressed(int key)
{
	if(key == 27) {// Escape code
		bPlay = false;

    } else if (key == 'w') {
        if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        wireframe = !wireframe;
    } else if (key == '1') {
        if (currentState == GAME_MENU) currentState = GAME_PLAY;
        hud.showTimeLeft();
        changeScene(0);
    } else if (key == '2') {
        if (currentState == GAME_MENU) currentState = GAME_PLAY;
        hud.showTimeLeft();
        changeScene(1);
    } else if (key == 'm') {
        currentState = GAME_MENU;
        menu.setMenuState(MainMenu::MenuState::TITLE);
        hud.hideTimeLeft();

        TileMap *newTileMap = menu.getMap();

        // la posicion del player se deberia coger de la escena. cada escena deberia guardar la posicion inicial del jugador
        player->setPosition(glm::vec2(menu.getInitPlayerTiles().x * newTileMap->getTileSize(), menu.getInitPlayerTiles().y * newTileMap->getTileSize()));
        player->setTileMap(newTileMap);
        camera.setPosition(glm::vec2(0, 0));
    } else if (key == 'l') {
        showsLoadingScene = !showsLoadingScene;
    }

	keys[key] = true;
}

void Game::changeScene(int sceneIndex) {
    currentSceneIndex = sceneIndex;
    Scene *newScene = scenes[currentSceneIndex];
    hud.setWorldNumber(newScene->getWorldNumber());

    if (showsLoadingScene) {
        stopRenderingPlayer();
        currentState = GAME_LOADING;
        loadingScene.setWorldNumber(newScene->getWorldNumber());
        loadingScene.start();
    } else {
        startRenderingPlayer();
        // Change the map on the player
        TileMap *newTileMap = newScene->getMap();
        TileMap *newBackgroundMap = newScene->getBackgroundMap();
        glm::ivec2 initPlayerTiles = newScene->getInitPlayerTiles();
        player->setPosition(glm::vec2(initPlayerTiles.x * newTileMap->getTileSize(), initPlayerTiles.y * newTileMap->getTileSize()));
        player->setTileMap(newTileMap);
        player->setBackgroundMap(newBackgroundMap);

        SoundEngine::instance().stopAllSounds();
        SoundEngine::instance().playMainTheme();
    }
}

void Game::startRenderingPlayer() {
    isRenderingPlayer = true;
}

void Game::stopRenderingPlayer() {
    isRenderingPlayer = false;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}


void Game::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	shaderProgram.init();
	shaderProgram.addShader(vShader);
	shaderProgram.addShader(fShader);
	shaderProgram.link();
	if(!shaderProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << shaderProgram.log() << endl << endl;
	}
	shaderProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



