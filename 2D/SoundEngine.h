#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <irrKlang/irrKlang.h>

class SoundEngine {
private:
    SoundEngine() {}


public:
    static SoundEngine &instance()
    {
        static SoundEngine S;

        return S;
    }

    ~SoundEngine();

    void init();

    void stopAllSounds();

    void playMainTheme(float playbackSpeed = 1);
    void stopMainTheme();
    void playJump();
    void playJumpBig();
    void playStageClear();
    void playFlagpole();
    void playStomp();
    void playKick();
    void playBreakBlock();
    void playDie();
    void playCoin();
    void playGameOver();
    void playBump();
    void playHurryUp();
    void playGrow();
    void playPowerup();
    void playPowerdown();

    bool isMainThemePlaying() { return !mainTheme->getIsPaused(); }

private:
    irrklang::ISoundEngine *engine;

    irrklang::ISound *mainTheme;
    irrklang::ISoundSource *jump;
    irrklang::ISoundSource *die;
    irrklang::ISoundSource *coin;
    irrklang::ISoundSource *game_over;
    irrklang::ISoundSource *stomp;
    irrklang::ISoundSource *grow;
    irrklang::ISoundSource *jump_big;
    irrklang::ISoundSource *stage_clear;
    irrklang::ISoundSource *flagpole;
    irrklang::ISoundSource *kick;
    irrklang::ISoundSource *break_block;
    irrklang::ISoundSource *bump;
    irrklang::ISoundSource *hurry_up;
    irrklang::ISoundSource *powerup;
    irrklang::ISoundSource *powerdown;

    bool isPlayingSound;

};

#endif // !SOUND_H
