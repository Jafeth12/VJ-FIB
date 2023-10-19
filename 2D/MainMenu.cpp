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

    currentState = MENU_TITLE;
    currentOptionSelected = 0;

    texProgram = &shaderProgram;

    logoTexture.loadFromFile("images/logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
    logoTexture.setMinFilter(GL_NEAREST);
    logoTexture.setMagFilter(GL_NEAREST);
    // mushroomTexture.loadFromFile("images/mushroom.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // checkar las dimensiones, las ha generado copilot y pueden estar mal
    logo = Sprite::createSprite(glm::ivec2(384, 192), glm::vec2(1.f, 1.f), &logoTexture, &shaderProgram);
    logo->setPosition(glm::vec2(64, 64));
    // mushroom = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &mushroomTexture, &shaderProgram);

    texts["optionPlay"] = Text::createText("1 PLAYER GAME", &shaderProgram, glm::vec2(10, 19));
    texts["optionTutorial"] = Text::createText("HOW TO PLAY", &shaderProgram, glm::vec2(11, 21));
    texts["optionCredits"] = Text::createText("CREDITS", &shaderProgram, glm::vec2(13, 23));
    texts["topScore"] = Text::createText("TOP- " + zeroFill(topScore, 6), &shaderProgram, glm::vec2(11, 26));
}


void MainMenu::update(float deltaTime) {

	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
        changeOptionUp();
    } else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
        changeOptionDown();
    }

}

void MainMenu::render() {
    Scene::render();

    logo->render();

}

void MainMenu::changeOptionUp() {
    if (currentOptionSelected == 0) return;
    --currentOptionSelected;
}

void MainMenu::changeOptionDown() {
    ++currentOptionSelected;
    if (currentOptionSelected == _LAST) currentOptionSelected = _LAST-1;
}

