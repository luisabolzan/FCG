#include "audio.h"
#include "globals.h"
#include <iostream>

void AudioInit() {
    ma_engine_init(NULL, &g_AudioEngine);
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/LadyJane.mp3", MA_SOUND_FLAG_STREAM, NULL, NULL, &g_Music);
    ma_sound_set_volume(&g_Music, 0.2f);
    ma_sound_set_looping(&g_Music, MA_TRUE);
    ma_sound_start(&g_Music);

    // Explosão
    ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/Explosion.mp3", 0, NULL, NULL, &g_SoundExplosion);
    ma_sound_set_volume(&g_SoundExplosion, 2.0f); 

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


void AudioMute() {
    g_IsMuted = !g_IsMuted;

    if (g_IsMuted) {
        ma_engine_set_volume(&g_AudioEngine, 0.0f);
    } else {
        ma_engine_set_volume(&g_AudioEngine, 1.0f);
    }
}

void AudioCleanup() {
    ma_sound_uninit(&g_Music);
    ma_sound_uninit(&g_SoundAccP1);
    ma_sound_uninit(&g_SoundDecelP1);
    ma_sound_uninit(&g_SoundAccP2);
    ma_sound_uninit(&g_SoundDecelP2);
    ma_sound_uninit(&g_SoundExplosion);

    ma_engine_uninit(&g_AudioEngine);
}

void AudioCoinSound() {
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Coin.mp3", NULL);
}

void AudioShotSound() {
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Shot.mp3", NULL);
}

void AudioExplosionSound(){
    if (ma_sound_is_playing(&g_SoundExplosion)) {
        ma_sound_stop(&g_SoundExplosion);
        ma_sound_seek_to_pcm_frame(&g_SoundExplosion, 0);
    }

    ma_sound_start(&g_SoundExplosion);
}

void AudioAccelerationSound(){
    ma_engine_play_sound(&g_AudioEngine, "../../data/audio/Acceleration.mp3", NULL);
}