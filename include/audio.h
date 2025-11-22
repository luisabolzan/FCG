#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"
#include "globals.h"

void Audio_Init();
void Audio_ToggleMute();
void Audio_Cleanup();

#endif // AUDIO_H