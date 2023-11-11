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
    stomp = engine->addSoundSourceFromFile("sounds/stomp.wav");
    jump_big = engine->addSoundSourceFromFile("sounds/jump_super.wav");
    stage_clear = engine->addSoundSourceFromFile("sounds/stage_clear.wav");
    flagpole = engine->addSoundSourceFromFile("sounds/flagpole.wav");
    kick = engine->addSoundSourceFromFile("sounds/kick.wav");
    break_block = engine->addSoundSourceFromFile("sounds/breakblock.wav");
    bump = engine->addSoundSourceFromFile("sounds/bump.wav");
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

void SoundEngine::playJumpBig() {
    engine->play2D(jump_big, false);
    isPlayingSound = true;
}

void SoundEngine::playFlagpole() {
    engine->play2D(flagpole, false);
    isPlayingSound = true;
}

void SoundEngine::playStageClear() {
    engine->play2D(stage_clear, false);
    isPlayingSound = true;
}

void SoundEngine::playStomp() {
    engine->play2D(stomp, false);
    isPlayingSound = true;
}

void SoundEngine::playKick() {
    engine->play2D(kick, false);
    isPlayingSound = true;
}

void SoundEngine::playDie() {
    engine->play2D(die, false);
    isPlayingSound = true;
}

void SoundEngine::playBreakBlock() {
    engine->play2D(break_block, false);
    isPlayingSound = true;
}

void SoundEngine::playBump() {
    engine->play2D(bump, false);
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

