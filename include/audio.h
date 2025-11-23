#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"
#include "globals.h"

void AudioInit();
void AudioMute();
void AudioCleanup();
void AudioCoinSound();
void AudioShotSound();
void AudioExplosionSound();
void AudioAccelerationSound();

#endif // AUDIO_H