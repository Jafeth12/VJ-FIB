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

    currentState = MenuState::MENU_TITLE;
    currentOptionSelected = 0;

    texProgram = &shaderProgram;

    logoTexture.loadFromFile("images/logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
    logoTexture.setMinFilter(GL_NEAREST);
    logoTexture.setMagFilter(GL_NEAREST);

    cursorTexture.loadFromFile("images/mushroom.png", TEXTURE_PIXEL_FORMAT_RGBA);
    cursorTexture.setMinFilter(GL_NEAREST);
    cursorTexture.setMagFilter(GL_NEAREST);

    // checkar las dimensiones, las ha generado copilot y pueden estar mal
    logo = Sprite::createSprite(glm::ivec2(384, 192), glm::vec2(1.f, 1.f), &logoTexture, &shaderProgram);
    logo->setPosition(glm::vec2(64, 64));


    texts["optionPlay"] = Text::createText("1 PLAYER GAME", &shaderProgram, glm::vec2(MENU_TEXT_POS_X, MENU_TEXT_POS_Y));
    texts["optionTutorial"] = Text::createText("HOW TO PLAY", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+1, MENU_TEXT_POS_Y+2));
    texts["optionCredits"] = Text::createText("CREDITS", &shaderProgram, glm::vec2(MENU_TEXT_POS_X+3, MENU_TEXT_POS_Y+4));
    texts["topScore"] = Text::createText("TOP- " + zeroFill(topScore, 6), &shaderProgram, glm::vec2(MENU_TEXT_POS_X+1, MENU_TEXT_POS_Y+7));

    cursor = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &cursorTexture, &shaderProgram);
    cursor->setPosition(glm::vec2(10*16 - 32, 18*16));
    cursor->setPosition(glm::vec2(MENU_CURSOR_POS_X*FONT_SIZE, MENU_CURSOR_POS_Y*FONT_SIZE));
}


void MainMenu::update(float deltaTime) {

    // [código escrito por chat jipitty]

    // static float keyPressTime = 0.0f;  // Initialize a timer for key presses.
    // const float keyPressDelay = 0.1f;  // Set the delay between key presses (adjust as needed).

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
                        currentState = MenuState::MENU_PLAY;
                        break;
                    case (char)MenuOption::MENU_OPTION_TUTORIAL:
                        currentState = MenuState::MENU_TUTORIAL;
                        break;
                    case (char)MenuOption::MENU_OPTION_CREDITS:
                        currentState = MenuState::MENU_CREDITS;
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

    logo->render();
    cursor->render();
}

void MainMenu::changeOptionUp() {
    if (currentOptionSelected == 0) return;
    --currentOptionSelected;
    cursor->setPosition(glm::vec2(MENU_CURSOR_POS_X*FONT_SIZE, MENU_CURSOR_POS_Y*FONT_SIZE + currentOptionSelected*2*FONT_SIZE));
}

void MainMenu::changeOptionDown() {
    ++currentOptionSelected;
    if (currentOptionSelected == (char)MenuOption::LAST) currentOptionSelected = (char)MenuOption::LAST - 1;
    cursor->setPosition(glm::vec2(MENU_CURSOR_POS_X*FONT_SIZE, MENU_CURSOR_POS_Y*FONT_SIZE + currentOptionSelected*2*FONT_SIZE));
}

