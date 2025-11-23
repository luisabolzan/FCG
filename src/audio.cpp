#include "audio.h"
#include "globals.h"
#include <iostream>

void Audio_Init() {
    ma_engine_init(NULL, &g_AudioEngine);
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/LadyJane.mp3", MA_SOUND_FLAG_STREAM, NULL, NULL, &g_Music);
    ma_sound_set_volume(&g_Music, 0.2f);
    ma_sound_set_looping(&g_Music, MA_TRUE);
    ma_sound_start(&g_Music);

    // Player 1
    // Aceleração
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/EngineOn.mp3", 0, NULL, NULL, &g_SoundAccP1);
    ma_sound_set_volume(&g_SoundAccP1, 0.5f);
    ma_sound_set_looping(&g_SoundAccP1, MA_TRUE);

    // Parada
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/EngineOff.mp3", 0, NULL, NULL, &g_SoundDecelP1);
    ma_sound_set_volume(&g_SoundDecelP1, 0.5f);
    ma_sound_set_looping(&g_SoundDecelP1, MA_TRUE);

    // Player 2
    // Aceleração
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/EngineOn.mp3", 0, NULL, NULL, &g_SoundAccP2);
    ma_sound_set_volume(&g_SoundAccP2, 0.5f);
    ma_sound_set_looping(&g_SoundAccP2, MA_TRUE);

    // Parada
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/EngineOff.mp3", 0, NULL, NULL, &g_SoundDecelP2);
    ma_sound_set_volume(&g_SoundDecelP2, 0.5f);
    ma_sound_set_looping(&g_SoundDecelP2, MA_TRUE);
}


void Audio_Mute() {
    g_IsMuted = !g_IsMuted;

    if (g_IsMuted) {
        ma_engine_set_volume(&g_AudioEngine, 0.0f);
    } else {
        ma_engine_set_volume(&g_AudioEngine, 1.0f);
    }
}

void Audio_Cleanup() {
    ma_sound_uninit(&g_Music);
    ma_sound_uninit(&g_SoundAccP1);
    ma_sound_uninit(&g_SoundDecelP1);
    ma_sound_uninit(&g_SoundAccP2);
    ma_sound_uninit(&g_SoundDecelP2);
    ma_engine_uninit(&g_AudioEngine);
}

void Audio_CoinSound() {
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Coin.mp3", NULL);
}

void Audio_ShotSound() {
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Shot.mp3", NULL);
}

void Audio_ExplosionSound(){
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Explosion.mp3", NULL);
}

void Audio_AccelerationSound(){
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Acceleration.mp3", NULL);
}