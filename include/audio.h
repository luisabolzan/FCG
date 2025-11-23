#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"
#include "globals.h"

void Audio_Init();
void Audio_Mute();
void Audio_Cleanup();
void Audio_CoinSound();
void Audio_ShotSound();

#endif // AUDIO_H