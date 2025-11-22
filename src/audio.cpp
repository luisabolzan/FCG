#include "audio.h"
#include "globals.h"
#include <iostream>

void Audio_Init() {
    if (ma_engine_init(NULL, &g_AudioEngine) != MA_SUCCESS) {
        std::cerr << "Falha ao iniciar audio engine.\n";
        return;
    }

    if (ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/LadyJane.mp3", 
        MA_SOUND_FLAG_STREAM, NULL, NULL, &g_Music) != MA_SUCCESS) {
        std::cerr << "Falha ao carregar musica.\n";
    }

    ma_sound_set_volume(&g_Music, 0.2f);
    ma_sound_set_looping(&g_Music, MA_TRUE);
    ma_sound_start(&g_Music);
}

void Audio_ToggleMute() {
    g_IsMuted = !g_IsMuted;

    if (g_IsMuted) {
        ma_engine_set_volume(&g_AudioEngine, 0.0f);
    } else {
        ma_engine_set_volume(&g_AudioEngine, 1.0f);
    }
}

void Audio_Cleanup() {
    ma_sound_uninit(&g_Music);
    ma_engine_uninit(&g_AudioEngine);
}