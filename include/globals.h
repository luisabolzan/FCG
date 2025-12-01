#ifndef GLOBALS_H
#define GLOBALS_H

#include <map>
#include <stack>
#include <glad/glad.h>
#include "FCGfunctions.h"
#include "miniaudio.h"

// =================================================================================
// CONSTANTES E DEFINIÇÕES
// =================================================================================

// Modelos de Iluminação
#define ILLUMINATION_GLOBAL     0
#define ILLUMINATION_LAMBERT    1
#define ILLUMINATION_PHONG      2
#define ILLUMINATION_BLINNPHONG 3

#define M_PI 3.14159265359

// IDs dos Objetos na Cena
#define SPHERE      0
#define BUNNY       1
#define PLANE       2
#define KART        3
#define RACETRACK   4
#define COIN        5
#define ROCKET      6
#define PALM        7
#define TREEL       8
#define TREEW       9

enum GameMode {
    MODE_SINGLEPLAYER, // 1 Jogador (Treino/Time Trial)
    MODE_MULTIPLAYER,  // 2 Jogadores (Split Screen)
    MODE_VERSUS_AI     // 1 Jogador vs Computador
};

// =================================================================================
// JANELA E CÂMERA
// =================================================================================
extern int g_ScreenWidth;
extern int g_ScreenHeight;
extern float g_ScreenRatio;
extern bool g_UsePerspectiveProjection;

// =================================================================================
// SHADERS E GPU
// =================================================================================
extern GLuint g_GpuProgramID;
extern GLint g_model_uniform;
extern GLint g_view_uniform;
extern GLint g_projection_uniform;
extern GLint g_object_id_uniform;
extern GLint g_object_type_uniform;
extern GLint g_bbox_min_uniform;
extern GLint g_bbox_max_uniform;

extern GLuint g_NumLoadedTextures;

// Configurações de Renderização Atuais
extern int IlluminationModel;
extern bool IsGouraudShading;

// =================================================================================
// SCENE E MATRIZES
// =================================================================================
extern std::map<std::string, struct SceneObject> g_VirtualScene;
extern std::stack<glm::mat4> g_MatrixStack;

// =================================================================================
// INPUTS (Mouse e Teclado)
// =================================================================================
// Mouse
extern bool g_LeftMouseButtonPressed;
extern bool g_RightMouseButtonPressed;
extern bool g_MiddleMouseButtonPressed;
extern double g_LastCursorPosX;
extern double g_LastCursorPosY;

// Teclado - Movimento
extern bool WPressed;
extern bool APressed;
extern bool SPressed;
extern bool DPressed;
extern bool UpArrowPressed;
extern bool DownArrowPressed;
extern bool LeftArrowPressed;
extern bool RightArrowPressed;
extern bool SpacePressed;
extern bool LeftShiftPressed;
extern bool RightShiftPressed;

// Teclado - Controle
extern bool CPressed;
extern bool MPressed;

// =================================================================================
// TEMPO E TIMER
// =================================================================================
extern float currentTime;
extern float deltaTime;
extern float lastTime;

extern float RoundTime;         // Tempo total configurado da rodada
extern float CurrentRoundTime;  // Tempo atual (decrescente)
extern bool GameEnded;

// =================================================================================
// ESTADOS DO JOGO (Gameplay)
// =================================================================================
extern bool g_ShowMenu;
extern GameMode g_CurrentGameMode;
extern bool g_ShowInfoText;

// =================================================================================
// ÁUDIO
// =================================================================================
extern bool g_IsMuted;
extern ma_engine g_AudioEngine;
extern ma_sound g_Music;
extern ma_sound g_SoundExplosion;
extern ma_sound g_SoundCoin;
// Player 1
extern ma_sound g_SoundAccP1;  
extern ma_sound g_SoundDecelP1; 
// Player 2
extern ma_sound g_SoundAccP2;
extern ma_sound g_SoundDecelP2;

// =================================================================================
// BEZIER
// =================================================================================
extern glm::vec3 p0;
extern glm::vec3 p1;
extern glm::vec3 p2;
extern glm::vec3 p3;

// =================================================================================
// OUTROS
// =================================================================================
extern float g_AngleX;
extern float g_AngleY;
extern float g_AngleZ;

#endif // GLOBALS_H