#include "menu.h"
#include <cstdio>

void RenderMenu(GLFWwindow* window)
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float char_width = TextRendering_CharWidth(window);

    // 1. Título "Smash Karts"
    const char* title_text = "SMASH KARTS";
    const float title_scale = 5.0f;
    float title_x_center = 0.0f - (char_width * strlen(title_text) * title_scale) / 2.0f;
    TextRendering_PrintString(window, title_text, title_x_center, 0.5f, title_scale);

    // 2. Botão "INICIAR"
    const char* start_text = "INICIAR JOGO (Pressione I)";
    const float start_scale = 1.0f;
    float start_x_center = 0.0f - (char_width * strlen(start_text) * start_scale) / 2.0f;
    TextRendering_PrintString(window, start_text, start_x_center, 0.1f, start_scale);

    // 3. Botão "SAIR"
    const char* exit_text = "SAIR (Pressione ESC)";
    const float exit_scale = 1.0f;
    float exit_x_center = 0.0f - (char_width * strlen(exit_text) * exit_scale) / 2.0f;
    TextRendering_PrintString(window, exit_text, exit_x_center, -0.1f, exit_scale);

    glEnable(GL_DEPTH_TEST);
}