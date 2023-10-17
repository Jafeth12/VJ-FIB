#ifndef STATS_H
#define STATS_H

#include <glm/glm.hpp>
#include <map>

#include "Text.h"

class StatsText {

public:
    // static StatsText &instance()
    // {
    //     static StatsText STATS;
    //
    //     return STATS;
    // }

    void init(ShaderProgram &shaderProgram);
    // void update(int value);
    void render();

    void setScore(int score);
    void setCoins(int coins);

    void setWorldNumber(int worldNumber);
    void incrementWorldNumber();

    void setTimeLeft(int timeLeft);
    void decrementTimeLeft();
    bool isTimeLeftZero() { return timeLeft == 0; }

    void showTimeLeft() { renderTimeLeft = true; }
    void hideTimeLeft() { renderTimeLeft = false; }

private:
    ShaderProgram *texProgram;

    std::map<std::string, Text*> texts;

    int score;
    int coins;
    int timeLeft;
    int worldNumber;

    bool renderTimeLeft;

    std::string zeroFill(int value, size_t digits);
};

#endif
