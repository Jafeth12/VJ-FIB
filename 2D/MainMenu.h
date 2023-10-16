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
    MENU_OPTION_EXIT,
};

class MainMenu {

public:
    MainMenu(ShaderProgram& program);
    ~MainMenu();

    void setMenuState(MenuState newState);
    void setScore(int score);
    void setCoins(int coins);
    void setTopScore(int topScore);

    void changeOptionUp();
    void changeOptionDown();

    void render();

private:
    ShaderProgram* shaderProgram;

    TileMap* tileMap;
    TileMap* background;
    TileMap* foreground;

    Sprite* logo;
    Texture logoTexture;

    Sprite* mushroom;
    Texture mushroomTexture;

    int score;
    int coins;
    int topScore;

    std::map<std::string, Text*> texts;

    MenuState currentState;
    MenuOption currentOptionSelected;
};

#endif
