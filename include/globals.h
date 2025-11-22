
#ifndef GLOBALS_H
#define GLOBALS_H

#include <map>
#include <stack>
#include <glad/glad.h>

#include "FCGfunctions.h"

#include "miniaudio.h"

extern std::map<std::string, struct SceneObject> g_VirtualScene;
extern std::stack<glm::mat4>  g_MatrixStack;
extern float g_ScreenRatio;
extern int g_ScreenWidth;
extern int g_ScreenHeight;

extern float g_AngleX;
extern float g_AngleY;
extern float g_AngleZ;

extern bool g_LeftMouseButtonPressed;
extern bool g_RightMouseButtonPressed;
extern bool g_MiddleMouseButtonPressed;

extern bool g_UsePerspectiveProjection;
extern bool g_ShowInfoText;

extern GLuint g_GpuProgramID;
extern GLint g_model_uniform;
extern GLint g_view_uniform;
extern GLint g_projection_uniform;
extern GLint g_object_id_uniform;
extern GLint g_object_type_uniform;
extern GLint g_bbox_min_uniform;
extern GLint g_bbox_max_uniform;

extern GLuint g_NumLoadedTextures;

extern double g_LastCursorPosX, g_LastCursorPosY;

// =================================================================================

extern bool g_ShowMenu;
extern bool isMultiplayer;

extern int IlluminationModel;
extern bool IsGouraudShading;

#define ILLUMINATION_GLOBAL     0
#define ILLUMINATION_LAMBERT    1
#define ILLUMINATION_PHONG      2
#define ILLUMINATION_BLINNPHONG 3

extern float currentTime;
extern float deltaTime;
extern float lastTime;

extern bool WPressed;
extern bool APressed;
extern bool SPressed;
extern bool DPressed;

extern bool UpArrowPressed;
extern bool DownArrowPressed;
extern bool LeftArrowPressed;
extern bool RightArrowPressed;

extern bool CPressed;
extern bool MPressed;
extern bool SpacePressed;
extern bool LeftShiftPressed;
extern bool RightShiftPressed;

// ==================================================================================
extern glm::vec3 p0;
extern glm::vec3 p1;
extern glm::vec3 p2;
extern glm::vec3 p3;


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

// Váriaveis de áudio
extern ma_engine g_AudioEngine;
extern ma_sound g_Music;

#endif //GLOBALS_H

