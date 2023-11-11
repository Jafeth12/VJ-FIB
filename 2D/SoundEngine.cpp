#include "SoundEngine.h"

void SoundEngine::init() {
    engine = irrklang::createIrrKlangDevice();

    if (!engine) {
        std::cout << "Error creating sound engine" << std::endl;
    }

    mainTheme = engine->play2D("sounds/main_theme.wav", true, true, true);
    mainTheme->setIsPaused(true);
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
    hurry_up = engine->addSoundSourceFromFile("sounds/hurry.wav");
    // hurry_up = engine->addSoundSourceFromFile("sounds/warning_time.wav");
}

SoundEngine::~SoundEngine() {
    engine->drop();
}

void SoundEngine::stopAllSounds() {
    if (isPlayingSound) engine->stopAllSounds();
    isPlayingSound = false;
}

void SoundEngine::playMainTheme(float playbackSpeed) {
    mainTheme = engine->play2D("sounds/main_theme.wav", true, false, true);
    mainTheme->setPlaybackSpeed(playbackSpeed);
    // engine->play2D(mainTheme, true);
    isPlayingSound = true;
}

void SoundEngine::stopMainTheme() {
    mainTheme->setIsPaused(true);
    isPlayingSound = false;
}

void SoundEngine::playHurryUp() {
    engine->play2D(hurry_up, false);
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

