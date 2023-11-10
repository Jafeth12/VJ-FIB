#include "SoundEngine.h"

void SoundEngine::init() {
    engine = irrklang::createIrrKlangDevice();

    if (!engine) {
        std::cout << "Error creating sound engine" << std::endl;
    }

    mainTheme = engine->addSoundSourceFromFile("sounds/main_theme.wav");
    jump = engine->addSoundSourceFromFile("sounds/jump.wav");
    die = engine->addSoundSourceFromFile("sounds/die.wav");
    coin = engine->addSoundSourceFromFile("sounds/coin.wav");
    game_over = engine->addSoundSourceFromFile("sounds/game_over.wav");
}

SoundEngine::~SoundEngine() {
    engine->drop();
}

void SoundEngine::stopAllSounds() {
    if (isPlayingSound) engine->stopAllSounds();
    isPlayingSound = false;
}

void SoundEngine::playMainTheme() {
    engine->play2D(mainTheme, true);
    isPlayingSound = true;
}

void SoundEngine::playJump() {
    engine->play2D(jump, false);
    isPlayingSound = true;
}

void SoundEngine::playDie() {
    engine->play2D(die, false);
    isPlayingSound = true;
}

void SoundEngine::playCoin() {
    engine->play2D(coin, false);
    isPlayingSound = true;
}

void SoundEngine::playGameOver() {
    engine->play2D(game_over, false);
    isPlayingSound = true;
}
