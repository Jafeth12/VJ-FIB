#include "StatsText.h"

void StatsText::init(ShaderProgram &shaderProgram) {
    texProgram = &shaderProgram;

    score = 0;
    coins = 0;
    timeLeft = 400;
    worldNumber = 1;
    renderTimeLeft = false;

    texts["mario"] = Text::createText("Mario", &shaderProgram, glm::vec2(3, 1));
    texts["score"] = Text::createText(zeroFill(score, 6), &shaderProgram, glm::vec2(3, 2));
    texts["coins"] = Text::createText("0x" + zeroFill(coins, 2), &shaderProgram, glm::vec2(12, 2));
    texts["worldText"] = Text::createText("World", &shaderProgram, glm::vec2(19, 1));
    texts["worldNumber"] = Text::createText("1-" + std::to_string(worldNumber), &shaderProgram, glm::vec2(20, 2));
    texts["timeText"] = Text::createText("Time", &shaderProgram, glm::vec2(25, 1));
    texts["timeNumber"] = Text::createText(zeroFill(timeLeft, 3), &shaderProgram, glm::vec2(26, 2));
}

void StatsText::setCoins(int coins) {
    this->coins = coins;
    texts["coins"]->updateText("0x" + zeroFill(coins, 2));
}

void StatsText::setScore(int score) {
    this->score = score;
    texts["score"]->updateText(zeroFill(score, 6));
}

void StatsText::setWorldNumber(int worldNumber) {
    this->worldNumber = worldNumber;
    texts["worldNumber"]->updateText("1-" + std::to_string(worldNumber));
}

void StatsText::incrementWorldNumber() {
    setWorldNumber(worldNumber + 1);
}

void StatsText::setTimeLeft(int timeLeft) {
    this->timeLeft = timeLeft;
    texts["timeNumber"]->updateText(zeroFill(timeLeft, 3));
}

void StatsText::decrementTimeLeft() {
    setTimeLeft(timeLeft - 1);
}

void StatsText::render() {
    for (auto it = texts.begin(); it != texts.end(); ++it) {
        if (!renderTimeLeft && it->first == "timeNumber") continue;

        it->second->render();
    }
}

// private
std::string StatsText::zeroFill(int value, size_t digits) {
    std::string result = std::to_string(value);

    while (result.length() < digits) {
        result = "0" + result;
    }

    return result;
}

