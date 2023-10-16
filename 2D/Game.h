#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Player.h"
#include "TileMap.h"
#include "Text.h"


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}

    void initShaders();
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(float deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

    void changeScene(int sceneIndex);

private:
    bool wireframe;
	bool bPlay;                       // Continue to play game?
	Scene sceneold;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
    Camera camera;
    ShaderProgram shaderProgram;
    std::vector<Scene*> scenes;
    int currentSceneIndex;
    Player *player;
};


#endif // _GAME_INCLUDE


