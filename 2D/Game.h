#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Player.h"
#include "TileMap.h"
#include "Text.h"
#include "HUD.h"
#include "MainMenu.h"
#include "LoadingScene.h"
#include "SoundEngine.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

#define SCORE_STOMP 100
#define SCORE_COIN 100
#define SCORE_POWERUP 1000
#define SCREEN_X 0
#define SCREEN_Y 16


// Game is a singleton (a class with a single instance) that represents our whole application

enum GameState {
    GAME_MENU = 0,
    GAME_PLAY,
    GAME_LOADING,
};

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

    ~Game();
	
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

    void startRenderingPlayer();
    void stopRenderingPlayer();

    void addScore(int score);

private:
    bool wireframe;
	bool bPlay;                       // Continue to play game?
	Scene sceneold;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
    Camera camera;
    HUD hud;
    MainMenu menu;
    ShaderProgram shaderProgram;
    std::vector<Scene*> scenes;

    bool showsLoadingScene;
    LoadingScene loadingScene;

    int currentSceneIndex;
    GameState currentState;
    Player *player;
    bool isRenderingPlayer;

    int totalScore;
    int lives;
};


#endif // _GAME_INCLUDE


