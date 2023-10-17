#ifndef MENU_H
#define MENU_H


#include "Scene.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Text.h"
#include <map>
#include <vector>
#include <string>

enum MenuState {
    MENU_TITLE = 0,
    MENU_TUTORIAL,
    MENU_CREDITS,
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

	virtual void init(ShaderProgram &shaderProgram, Camera &camera, StatsText &statsText, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords);
    void update(float deltaTime);
    virtual void render();

    void setMenuState(MenuState newState);

    void changeOptionUp();
    void changeOptionDown();

private:
    Sprite* logo;
    Texture logoTexture;

    Sprite* mushroom;
    Texture mushroomTexture;

    int topScore;

    MenuState currentState;
    char currentOptionSelected;
};

#endif
