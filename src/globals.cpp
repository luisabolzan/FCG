#include "miniaudio.h"
#include "globals.h"

// =================================================================================
// JANELA E CÂMERA
// =================================================================================
int g_ScreenWidth = 1920;
int g_ScreenHeight = 1080;
float g_ScreenRatio = 1920.0f / 1080.0f;
bool g_UsePerspectiveProjection = true;

// =================================================================================
// SHADERS E GPU
// =================================================================================
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;
GLint g_object_type_uniform;
GLint g_bbox_min_uniform;
GLint g_bbox_max_uniform;

GLuint g_NumLoadedTextures = 0;

int IlluminationModel = ILLUMINATION_GLOBAL;
bool IsGouraudShading = false;

// =================================================================================
// SCENE E MATRIZES
// =================================================================================
std::map<std::string, SceneObject> g_VirtualScene;
std::stack<glm::mat4> g_MatrixStack;

// =================================================================================
// INPUTS (Mouse e Teclado)
// =================================================================================
// Mouse
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false;
bool g_MiddleMouseButtonPressed = false;
double g_LastCursorPosX = 0.0;
double g_LastCursorPosY = 0.0;

// Teclado - Movimento
bool WPressed = false;
bool APressed = false;
bool SPressed = false;
bool DPressed = false;

bool UpArrowPressed = false;
bool DownArrowPressed = false;
bool LeftArrowPressed = false;
bool RightArrowPressed = false;

bool SpacePressed = false;
bool LeftShiftPressed = false;
bool RightShiftPressed = false;

// Teclado - Controle
bool CPressed = false;
bool MPressed = false;

// =================================================================================
// TEMPO E TIMER
// =================================================================================
float currentTime = 0.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;

float RoundTime = 60.0f;
float CurrentRoundTime = 0.0f;
bool GameEnded = false;

// =================================================================================
// ESTADOS DO JOGO (Gameplay)
// =================================================================================
bool g_ShowMenu = true;
bool isMultiplayer = false;
bool g_ShowInfoText = false;

// =================================================================================
// ÁUDIO
// =================================================================================
bool g_IsMuted = false;
ma_engine g_AudioEngine;
ma_sound g_Music;
ma_sound g_SoundExplosion;
ma_sound g_SoundCoin;
ma_sound g_SoundAccP1;
ma_sound g_SoundDecelP1;
ma_sound g_SoundAccP2;
ma_sound g_SoundDecelP2;

// =================================================================================
// BEZIER
// =================================================================================
glm::vec3 p0(0.0f, 0.0f, 0.0f);
glm::vec3 p1(2.0f, 0.0f, 2.0f);
glm::vec3 p2(4.0f, 0.0f, 4.0f);
glm::vec3 p3(6.0f, 0.0f, 0.0f);

// =================================================================================
// OUTROS
// =================================================================================
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;