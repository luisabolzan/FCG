#include <glad/glad.h>
#include "gameover.h"
#include "FCGfunctions.h"
#include "globals.h"      
#include <cstdio>
#include <cstring>
#include <glad/glad.h>

void RenderGameOverScreen(GLFWwindow* window, int scoreP1, int scoreP2) {
    glDisable(GL_DEPTH_TEST);
    
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

    const char* msgResult;
    if (scoreP1 > scoreP2) {
        msgResult = "VENCEDOR: PLAYER 1";
    } else if (scoreP2 > scoreP1) {
        msgResult = "VENCEDOR: PLAYER 2";
    } else {
        msgResult = "EMPATE!";
    }

    const char* msgRestart = "PRESSIONE 'I' PARA VOLTAR AO MENU";

    float char_width = TextRendering_CharWidth(window);
    
    float scaleBig = 2.0f;
    float widthResult = strlen(msgResult) * char_width * scaleBig;
    float x_result = 0.0f - (widthResult / 2.0f);

    float scaleSmall = 2.0f;
    float widthRestart = strlen(msgRestart) * char_width * scaleSmall;
    float x_restart = 0.0f - (widthRestart / 2.0f);

    TextRendering_PrintString(window, msgResult, x_result, 0.2f, scaleBig);
    TextRendering_PrintString(window, msgRestart, x_restart, -0.3f, scaleSmall);

    glEnable(GL_DEPTH_TEST);
}