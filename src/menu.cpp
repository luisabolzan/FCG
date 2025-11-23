#include "menu.h"
#include "globals.h"
#include <cstdio>
#include <cstring>

void RenderAudioStatus(GLFWwindow* window) {
    glDisable(GL_DEPTH_TEST); 

    const char* audio_text;
    if (g_IsMuted) {
        audio_text = "SONG: OFF [X]";
    } else {
        audio_text = "SONG: ON  [X]";
    }

    float audio_x = 0.6f; 
    float audio_y = 0.85f;
    float audio_scale = 2.0f;

    TextRendering_PrintString(window, audio_text, audio_x, audio_y, audio_scale);

    glEnable(GL_DEPTH_TEST); 
}

void RenderMenu(GLFWwindow* window) {
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);

    float char_width = TextRendering_CharWidth(window);

    // Título
    const char* title_text = "SMASH KARTS";
    const float title_scale = 5.0f;
    float title_x_center = 0.0f - (char_width * strlen(title_text) * title_scale) / 2.0f;
    TextRendering_PrintString(window, title_text, title_x_center, 0.5f, title_scale);

    // Opção Singleplayer
    const char* single_text = "1 - SINGLEPLAYER";
    const float scale_opts = 3.0f;
    float single_x = 0.0f - (char_width * strlen(single_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, single_text, single_x, 0.1f, scale_opts);

    // Opção Multiplayer
    const char* multi_text = "2 - MULTIPLAYER (SPLIT SCREEN)";
    float multi_x = 0.0f - (char_width * strlen(multi_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, multi_text, multi_x, -0.05f, scale_opts);

    // Sair
    const char* exit_text = "ESC - SAIR";
    float exit_x = 0.0f - (char_width * strlen(exit_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, exit_text, exit_x, -0.3f, scale_opts);    

    RenderAudioStatus(window);

    glEnable(GL_DEPTH_TEST);
}