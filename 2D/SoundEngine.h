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

    void playUnderworldTheme();
    void stopUnderworldTheme();

    void pauseMainTheme();
    void resumeMainTheme();

    void pauseUnderworldTheme();
    void resumeUnderworldTheme();

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
    void playBeep();
    void playPause();

private:
    irrklang::ISoundEngine *engine;

    irrklang::ISound *mainTheme;
    irrklang::ISound *underworldTheme;
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
    irrklang::ISoundSource *pause;
    irrklang::ISoundSource *beep;

    bool isPlayingSound;

};

#endif // !SOUND_H
