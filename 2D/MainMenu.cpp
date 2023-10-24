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

void MainMenu::init(ShaderProgram &shaderProgram, Camera &camera, HUD &hud, std::string levelFilename, glm::ivec2 initPlayerTiles, glm::ivec2 minCoords) {
    Scene::init(shaderProgram, camera, hud, levelFilename, initPlayerTiles, minCoords);

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
}

void MainMenu::update(float deltaTime) {

    // [código escrito por chat jipitty]
    bool up = Game::instance().getSpecialKey(GLUT_KEY_UP);
    bool down = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
    bool enter = Game::instance().getKey(13);

    if (up || down || enter) {
        // Get the current time.
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

        // Check if enough time has passed since the last key press.
        if (currentTime - keyPressTime >= MENU_KEY_PRESS_DELAY) {

            if (up) {
                changeOptionUp();
            } else if (down) {
                changeOptionDown();
            } else if (enter) {

                switch (currentOptionSelected) {
                    case (char)MenuOption::MENU_OPTION_PLAY:
                        currentState = MenuState::PLAY;
                        break;
                    case (char)MenuOption::MENU_OPTION_TUTORIAL:
                        currentState = MenuState::TUTORIAL;
                        break;
                    case (char)MenuOption::MENU_OPTION_CREDITS:
                        currentState = MenuState::CREDITS;
                        break;
                }
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
    cursor.sprite->render();
}

glm::vec2 MainMenu::posFromOption(char option) {
    return glm::vec2(MENU_CURSOR_POS_X*FONT_SIZE, MENU_CURSOR_POS_Y*FONT_SIZE + option*2*FONT_SIZE);
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

