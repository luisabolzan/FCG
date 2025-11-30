#ifndef GAMETEXT_H
#define GAMETEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "globals.h"
#include "kart.h"
#include "FCGfunctions.h"

// =============================================================================
// 1. TELAS PRINCIPAIS (Menus e Telas Cheias)
// =============================================================================

// Renderiza o Menu Principal
void RenderMenu(GLFWwindow* window);

// Renderiza a Tela de Fim de Jogo
void RenderGameOver(GLFWwindow* window, int scoreP1, int scoreP2);

// =============================================================================
// 2. HUDS DE JOGO (Interfaces completas durante a partida)
// =============================================================================

// HUD para Singleplayer (Tela cheia, timer grande)
void RenderSinglePlayerHUD(GLFWwindow* window, Kart& player1, Kart& player2, float roundTime);

// HUD para Multiplayer (Tela dividida, timer duplo pequeno)
void RenderMultiPlayerHUD(GLFWwindow* window, Kart& player1, Kart& player2, float roundTime);

// =============================================================================
// 3. COMPONENTES DA INTERFACE (Funções auxiliares de desenho)
// =============================================================================

// Timer da partida
void RenderMatchTimer(GLFWwindow* window, float roundTime, float x, float y, float scale);

// Informações do Kart (Nome, Vida, Munição, Score)
void RenderKartInfo(GLFWwindow* window, Kart& kart, float x_pos, float y_pos);

// Ranking
void RenderRanking(GLFWwindow* window, Kart& p1, Kart& p2, float x_pos, float y_pos);

// Velocímetro
void RenderSpeed(GLFWwindow* window, Kart& kart, float x_pos, float y_pos);

// Mensagem de morte/respawn e quem matou
void RenderRespawnMessage(GLFWwindow* window, Kart& target, std::string shooterName, float x_center, float y_center);

// Status do áudio (Mute/Unmute)
void RenderAudioStatus(GLFWwindow* window);

#endif // GAMETEXT_H