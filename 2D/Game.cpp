#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
    currentSceneIndex = 0;
	bPlay = true;
    wireframe = true;
	glClearColor(0.45, 0.45f, 1.0f, 1.0f);
    camera = Camera(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    initShaders();

    // create scenes
    scenes.push_back(new Scene());
    scenes.push_back(new Scene());
	scenes[currentSceneIndex]->init(shaderProgram, camera, "levels/level01.txt");
	scenes[currentSceneIndex+1]->init(shaderProgram, camera, "levels/level02.txt");
}

bool Game::update(int deltaTime)
{
    scenes[currentSceneIndex]->update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scenes[currentSceneIndex]->render();
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
        currentSceneIndex = 0;
    } else if (key == '2') {
        currentSceneIndex = 1;
    }
	keys[key] = true;
}

void Game::changeScene(int sceneIndex) {
    currentSceneIndex = sceneIndex;
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



