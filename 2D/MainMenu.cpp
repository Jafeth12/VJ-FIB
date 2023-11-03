#include "MainMenu.h"
#include "Game.h"
#include <GL/freeglut_std.h>

std::string zeroFill(int value, size_t digits) {
    std::string result = std::to_string(value);

    while (result.length() < digits) {
        result = "0" + result;
    }

    return result;
}

MainMenu::MainMenu() {
}

MainMenu::~MainMenu() {
}

MainMenu::MainMenu(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords) {
    MainMenu::init(shaderProgram, camera, hud, levelFilename, initPlayerTiles, minCoords);
}

void MainMenu::init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords) {
    Scene::init(shaderProgram, camera, hud, levelFilename, initPlayerTiles, minCoords);
    autoRenderAllText = false;

    currentState = MenuState::TITLE;
    currentOptionSelected = 0;

    texProgram = &shaderProgram;

    logo.texture.loadFromFile("images/logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
    logo.texture.setMinFilter(GL_NEAREST);
    logo.texture.setMagFilter(GL_NEAREST);

    cursor.texture.loadFromFile("images/mushroom.png", TEXTURE_PIXEL_FORMAT_RGBA);
    cursor.texture.setMinFilter(GL_NEAREST);
    cursor.texture.setMagFilter(GL_NEAREST);

    // TODO intentar quit magic numbers (384, 192), (32, 32), (64, 64)
    logo.sprite = Sprite::createSprite(glm::ivec2(384, 192), glm::vec2(1.f, 1.f), &logo.texture, &shaderProgram);
    logo.sprite->setPosition(glm::vec2(64, 64));

    cursor.sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &cursor.texture, &shaderProgram);
    cursor.sprite->setPosition(glm::vec2(MENU_CURSOR_POS_X*FONT_SIZE, MENU_CURSOR_POS_Y*FONT_SIZE));

    texts["optionPlay"] = Text::createText("1 PLAYER GAME", &shaderProgram, glm::vec2(MENU_TEXT_POS_X, MENU_TEXT_POS_Y));
    texts["optionTutorial"] = Text::createText("HOW TO PLAY", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+1, MENU_TEXT_POS_Y+2));
    texts["optionCredits"] = Text::createText("CREDITS", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+3, MENU_TEXT_POS_Y+4));
    texts["topScore"] = Text::createText("TOP- " + zeroFill(topScore, 6), &shaderProgram, glm::vec2(MENU_TEXT_POS_X+1, MENU_TEXT_POS_Y+7));

    // TEMPORAL, ESTO SERA UN IMAGEN
    texts["help"] = Text::createText("ARROW KEYS TO MOVE", &shaderProgram, glm::vec2(MENU_TEXT_POS_X-2, MENU_TEXT_POS_Y));
    texts["help1"] = Text::createText("SHIFT TO RUN", &shaderProgram, glm::vec2(MENU_TEXT_POS_X, MENU_TEXT_POS_Y+2));
    texts["help3"] = Text::createText("SPACE TO JUMP", &shaderProgram, glm::vec2(MENU_TEXT_POS_X, MENU_TEXT_POS_Y+4));
    texts["help4"] = Text::createText("BACKSPACE TO GO BACK IN MENUS", &shaderProgram, glm::vec2(MENU_TEXT_POS_X-8, MENU_TEXT_POS_Y+6));

    texts["credits1"] = Text::createText("MADE BY:", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+2, MENU_TEXT_POS_Y));
    texts["credits2"] = Text::createText("ELOI MERINO", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+1, MENU_TEXT_POS_Y+2));
    texts["credits3"] = Text::createText("&", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+5, MENU_TEXT_POS_Y+4));
    texts["credits4"] = Text::createText("JAFETH CONCEPCION", &shaderProgram, glm::vec2(MENU_TEXT_POS_X-2, MENU_TEXT_POS_Y+6));
}

void MainMenu::update(float deltaTime) {

    // [código escrito por chat jipitty]
    bool up = Game::instance().getSpecialKey(GLUT_KEY_UP);
    bool down = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
    bool enter = Game::instance().getKey(13);
    bool back = Game::instance().getKey(8);
    bool buttonPressed = up || down || enter || back;

    if (buttonPressed) {
        // Get the current time.
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

        // Check if enough time has passed since the last key press.
        if (currentTime - keyPressTime >= MENU_KEY_PRESS_DELAY) {

            switch (currentState) {
                case MenuState::TITLE:
                    if (up) {
                        changeOptionUp();
                    } else if (down) {
                        changeOptionDown();
                    } else if (enter) {

                        if (currentOptionSelected == (char)MenuOption::MENU_OPTION_PLAY) {
                            currentState = MenuState::PLAY;
                        } else if (currentOptionSelected == (char)MenuOption::MENU_OPTION_TUTORIAL) {
                            currentState = MenuState::TUTORIAL;
                        } else if (currentOptionSelected == (char)MenuOption::MENU_OPTION_CREDITS) {
                            currentState = MenuState::CREDITS;
                        }
                    }

                    break;
                case MenuState::TUTORIAL:
                    if (back) currentState = MenuState::TITLE;
                    break;
                case MenuState::CREDITS:
                    if (back) currentState = MenuState::TITLE;
                    break;
                case MenuState::PLAY:
                    break;
            }

            // Update the key press time to the current time.
            keyPressTime = currentTime;
        }
    }

}

void MainMenu::setMenuState(MenuState newState) {
    currentState = newState;
}

MainMenu::MenuState MainMenu::getMenuState() {
    return currentState;
}

void MainMenu::render() {
    Scene::render();

    logo.sprite->render();

    switch (currentState) {
        case MenuState::TITLE:
            texts["optionPlay"]->render();
            texts["optionTutorial"]->render();
            texts["optionCredits"]->render();
            texts["topScore"]->render();
            cursor.sprite->render();
            break;
        case MenuState::TUTORIAL:
            renderHelp();
            break;
        case MenuState::CREDITS:
            renderCredits();
            break;
        case MenuState::PLAY:
            break;
    }

}

glm::vec2 MainMenu::posFromOption(char option) {
    return glm::vec2(MENU_CURSOR_POS_X*FONT_SIZE, MENU_CURSOR_POS_Y*FONT_SIZE + option*2*FONT_SIZE);
}

void MainMenu::renderCredits() {
    texts["credits1"]->render();
    texts["credits2"]->render();
    texts["credits3"]->render();
    texts["credits4"]->render();
}

void MainMenu::renderHelp() {
    texts["help"]->render();
    texts["help1"]->render();
    texts["help3"]->render();
    texts["help4"]->render();
}

void MainMenu::changeOptionUp() {
    if (currentOptionSelected == 0) return;
    --currentOptionSelected;
    cursor.sprite->setPosition(posFromOption(currentOptionSelected));
}

void MainMenu::changeOptionDown() {
    ++currentOptionSelected;
    if (currentOptionSelected == (char)MenuOption::LAST) currentOptionSelected = (char)MenuOption::LAST - 1;
    cursor.sprite->setPosition(posFromOption(currentOptionSelected));
}

