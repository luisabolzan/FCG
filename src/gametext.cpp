#include "gametext.h"
#include <cmath>

// =============================================================================
// 1. TELAS PRINCIPAIS
// =============================================================================

void RenderMenu(GLFWwindow* window) {

    glDisable(GL_DEPTH_TEST);
    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

    float char_width = TextRendering_CharWidth(window);

    // Título
    const char* title_text = "SMASH KARTS";
    const float title_scale = 5.0f;
    float title_x_center = 0.0f - (char_width * strlen(title_text) * title_scale) / 2.0f;
    TextRendering_PrintString(window, title_text, title_x_center, 0.5f, title_scale);

    // Opções
    const char* single_text = "1 - SINGLEPLAYER";
    const float scale_opts = 3.0f;
    float single_x = 0.0f - (char_width * strlen(single_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, single_text, single_x, 0.1f, scale_opts);

    const char* multi_text = "2 - MULTIPLAYER (SPLIT SCREEN)";
    float multi_x = 0.0f - (char_width * strlen(multi_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, multi_text, multi_x, -0.05f, scale_opts);

    const char* ai_text = "3 - VERSUS AI (PLAYER VS CPU)";
    float ai_x = 0.0f - (char_width * strlen(ai_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, ai_text, ai_x, -0.20f, scale_opts);

    const char* exit_text = "ESC - SAIR";
    float exit_x = 0.0f - (char_width * strlen(exit_text) * scale_opts) / 2.0f;
    TextRendering_PrintString(window, exit_text, exit_x, -0.35f, scale_opts);

    RenderAudioStatus(window);
    glEnable(GL_DEPTH_TEST);
}

void RenderGameOver(GLFWwindow* window, int scoreP1, int scoreP2) {

    glDisable(GL_DEPTH_TEST);
    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

    const char* msgResult;
    if (scoreP1 > scoreP2)
        msgResult = "VENCEDOR: PLAYER 1";
    else if (scoreP2 > scoreP1)
        msgResult = "VENCEDOR: PLAYER 2";
    else
        msgResult = "EMPATE!";

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


// =============================================================================
// 2. HUDS DO JOGO
// =============================================================================

void RenderSinglePlayerHUD(GLFWwindow* window, Kart& player1, Kart& player2, float roundTime) {

    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);
    glDisable(GL_DEPTH_TEST);

    RenderMatchTimer(window, roundTime, 0.0f, 0.90f, 4.0f);
    RenderKartInfo(window, player1, -0.95f, 0.85f);
    RenderRanking(window, player1, player2, 0.75f, 0.8f);
    RenderSpeed(window, player1, -0.95f, -0.9f);
    RenderRespawnMessage(window, player1, "ENEMY", 0.0f, 0.15f);

    glEnable(GL_DEPTH_TEST);
}

void RenderMultiPlayerHUD(GLFWwindow* window, Kart& player1, Kart& player2, float roundTime) {

    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);
    glDisable(GL_DEPTH_TEST);

    // PLAYER 1 (Esquerda)
    RenderKartInfo(window, player1, -0.95f, 0.85f);
    RenderRanking(window, player1, player2, -0.20f, 0.8f);
    RenderMatchTimer(window, roundTime, -0.9f, 0.95f, 1.5f);
    RenderSpeed(window, player1, -0.95f, -0.9f);
    RenderRespawnMessage(window, player1, player2.name, -0.5f, 0.15f);

    // PLAYER 2 (Direita)
    RenderKartInfo(window, player2, 0.05f, 0.85f);
    RenderRanking(window, player1, player2, 0.80f, 0.8f);
    RenderMatchTimer(window, roundTime, 0.1f, 0.95f, 1.5f);
    RenderSpeed(window, player2, 0.05f, -0.9f);
    RenderRespawnMessage(window, player2, player1.name, 0.5f, 0.15f);

    glEnable(GL_DEPTH_TEST);
}


// =============================================================================
// 3. COMPONENTES DA INTERFACE
// =============================================================================

void RenderMatchTimer(GLFWwindow* window, float roundTime, float x_center, float y_pos, float scale) {

    char buffer[32];
    int t = (int)ceil(roundTime);
    if (t < 0) t = 0;

    snprintf(buffer, sizeof(buffer), "TIME: %02d", t);

    float char_width = TextRendering_CharWidth(window);
    float text_width = char_width * strlen(buffer) * scale;
    float final_x = x_center - (text_width / 2.0f);

    TextRendering_PrintString(window, buffer, final_x, y_pos, scale);
}

void RenderKartInfo(GLFWwindow* window, Kart& kart, float x_pos, float y_pos) {

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s | HP: %d | Ammo: %d | Score: %d",
             kart.name.c_str(), kart.health, kart.ammo, kart.score);

    float scale = 1.5f;
    TextRendering_PrintString(window, buffer, x_pos, y_pos, scale);
}

void RenderRanking(GLFWwindow* window, Kart& p1, Kart& p2, float x_pos, float y_pos) {

    char txtFirst[64];
    char txtSecond[64];
    float scale = 1.2f;

    Kart* primeiro;
    Kart* segundo;

    if (p1.score >= p2.score) {
        primeiro = &p1;
        segundo = &p2;
    } else {
        primeiro = &p2;
        segundo = &p1;
    }

    TextRendering_PrintString(window, "RANKING:", x_pos, y_pos + 0.1f, scale);

    snprintf(txtFirst, sizeof(txtFirst), "1. %s (%d)", primeiro->name.c_str(), primeiro->score);
    TextRendering_PrintString(window, txtFirst, x_pos, y_pos, scale);

    snprintf(txtSecond, sizeof(txtSecond), "2. %s (%d)", segundo->name.c_str(), segundo->score);
    TextRendering_PrintString(window, txtSecond, x_pos, y_pos - 0.1f, scale);
}

void RenderSpeed(GLFWwindow* window, Kart& kart, float x_pos, float y_pos) {

    char buffer[64];
    int speedVal = (int)(std::abs(kart.speed) * 3.6f);
    snprintf(buffer, sizeof(buffer), "SPEED: %02d km/h", speedVal);

    float scale = 3.0f;
    TextRendering_PrintString(window, buffer, x_pos, y_pos, scale);
}

void RenderRespawnMessage(GLFWwindow* window, Kart& target, std::string shooterName, float x_center, float y_center) {

    if (target.isAlive) return;

    int timeLeft = (int)(target.respawnTime - target.respawnTimer) + 1;

    char msgHit[64];
    char msgTimer[64];
    float scale = 3.0f;
    float char_width = TextRendering_CharWidth(window);

    snprintf(msgHit, sizeof(msgHit), "ELIMINADO POR %s", shooterName.c_str());
    snprintf(msgTimer, sizeof(msgTimer), "RESPAWN EM: %d", timeLeft);

    float widthHit = strlen(msgHit) * char_width * scale;
    float x_hit = x_center - (widthHit / 2.0f);

    float widthTimer = strlen(msgTimer) * char_width * scale;
    float x_timer = x_center - (widthTimer / 2.0f);

    TextRendering_PrintString(window, msgHit, x_hit, y_center, scale);
    TextRendering_PrintString(window, msgTimer, x_timer, y_center - 0.15f, scale);
}

void RenderAudioStatus(GLFWwindow* window) {

    glDisable(GL_DEPTH_TEST);

    const char* audio_text = g_IsMuted ? "SONG: OFF [X]" : "SONG: ON  [X]";
    float audio_x = 0.6f;
    float audio_y = 0.85f;
    float audio_scale = 2.0f;
    TextRendering_PrintString(window, audio_text, audio_x, audio_y, audio_scale);

    glEnable(GL_DEPTH_TEST);
}