#ifndef HUD_H
#define HUD_H

#include <glm/glm.hpp>
#include <map>

#include "Text.h"

class HUD {

public:
    // static HUD &instance()
    // {
    //     static HUD hud;
    //
    //     return hud;
    // }

    void init(ShaderProgram &shaderProgram);
    // void update(int value);
    void render();

    void setScore(int score);
    void setCoins(int coins);
    void addCoin();

    void setWorldNumber(int worldNumber);

    void setTimeLeft(int timeLeft);
    void decrementTimeLeft();
    bool isTimeLeftZero() { return timeLeft == 0; }
    int getTimeLeft() { return timeLeft; }

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
