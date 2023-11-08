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

    void playMainTheme();
    void playJump();
    void playDie();
    void playCoin();
    void playGameOver();

private:
    irrklang::ISoundEngine *engine;

    irrklang::ISoundSource *mainTheme;
    irrklang::ISoundSource *jump;
    irrklang::ISoundSource *die;
    irrklang::ISoundSource *coin;
    irrklang::ISoundSource *game_over;

};

#endif // !SOUND_H
