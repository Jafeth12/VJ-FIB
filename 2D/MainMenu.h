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

#define MENU_KEY_PRESS_DELAY 0.1f


class MainMenu : public Scene {

public:
    // MainMenu();
    // ~MainMenu();

    enum class MenuState {
        TITLE = 0,
        TUTORIAL,
        CREDITS,
        PLAY,
    };

	virtual void init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords);
    void update(float deltaTime);
    virtual void render();

    void setTopScore(int newTopScore);
    void setMenuState(MenuState newState);

    MenuState getMenuState();

    void changeOptionUp();
    void changeOptionDown();

private:

    enum class MenuOption {
        MENU_OPTION_PLAY = 0,
        MENU_OPTION_TUTORIAL,
        MENU_OPTION_CREDITS,
        LAST,
    };

    struct MenuSprite {
        Sprite* sprite;
        Texture texture;
    };

    MenuSprite logo;
    MenuSprite cursor;

    int topScore;

    MenuState currentState;
    char currentOptionSelected;

    float keyPressTime = 0.0f; // RIB=Rest in Beans

    glm::vec2 posFromOption(char option);
};

#endif
