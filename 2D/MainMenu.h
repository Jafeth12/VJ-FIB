#ifndef MENU_H
#define MENU_H


#include "Scene.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Text.h"
#include <map>
#include <vector>
#include <string>

#define MENU_TEXT_POS_X 10
#define MENU_TEXT_POS_Y 19

#define MENU_CURSOR_POS_X 8
#define MENU_CURSOR_POS_Y 18

enum MenuState {
    MENU_TITLE = 0,
    MENU_TUTORIAL,
    MENU_CREDITS,
    MENU_PLAY,
};

enum MenuOption {
    MENU_OPTION_PLAY = 0,
    MENU_OPTION_TUTORIAL,
    MENU_OPTION_CREDITS,
    _LAST,
};

class MainMenu : public Scene {

public:
    // MainMenu();
    // ~MainMenu();

	virtual void init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords);
    void update(float deltaTime);
    virtual void render();

    void setTopScore(int newTopScore);
    void setMenuState(MenuState newState);

    MenuState getMenuState();

    void changeOptionUp();
    void changeOptionDown();

private:
    Sprite* logo;
    Texture logoTexture;

    Sprite* cursor;
    Texture cursorTexture;

    int topScore;

    MenuState currentState;
    char currentOptionSelected;
};

#endif
