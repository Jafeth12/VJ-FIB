#include "MainMenu.h"

MainMenu::MainMenu(ShaderProgram& program) {
    currentState = MENU_TITLE;
    currentOptionSelected = MENU_OPTION_PLAY;

    shaderProgram = &program;

    logoTexture.loadFromFile("images/logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
    mushroomTexture.loadFromFile("images/mushroom.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // checkar las dimensiones, las ha generado copilot y pueden estar mal
    logo = Sprite::createSprite(glm::ivec2(256, 64), glm::vec2(1.f, 1.f), &logoTexture, &program);
    mushroom = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &mushroomTexture, &program);

    score = 0;
    coins = 0;
    topScore = 0;

    // hay que poner los trailing zeros para que se vea bien

    texts["mario"] = Text::createText("Mario", &program, glm::vec2(3, 1));
    texts["score"] = Text::createText("000000", &program, glm::vec2(3, 2));
    texts["coins"] = Text::createText("0x00", &program, glm::vec2(12, 2));
    texts["worldText"] = Text::createText("World", &program, glm::vec2(19, 1));
    texts["worldNumber"] = Text::createText("1-1", &program, glm::vec2(20, 2));
    texts["timeText"] = Text::createText("Time", &program, glm::vec2(25, 1));

    texts["topScore"] = Text::createText("TOP- " + std::to_string(topScore), &program, glm::vec2(3, 4));
    texts["optionPlay"] = Text::createText("1 PLAYER GAME", &program, glm::vec2(3, 6));
    texts["optionTutorial"] = Text::createText("HOW TO PLAY", &program, glm::vec2(3, 7));
    texts["optionCredits"] = Text::createText("CREDITS", &program, glm::vec2(3, 8));
    texts["optionExit"] = Text::createText("EXIT", &program, glm::vec2(3, 9));
}
