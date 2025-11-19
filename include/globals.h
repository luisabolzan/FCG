
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

extern int IlluminationModel;
extern bool IsGouraudShading;

extern float currentTime;
extern float deltaTime;
extern float lastTime;

extern bool WPressed;
extern bool APressed;
extern bool SPressed;
extern bool DPressed;

extern bool CPressed;
extern bool MPressed;

extern bool SpacePressed;
extern bool ShiftPressed;

// ==================================================================================
extern glm::vec3 p0;
extern glm::vec3 p1;
extern glm::vec3 p2;
extern glm::vec3 p3;


#define SPHERE      0
#define BUNNY       1
#define PLANE       2
#define KART       3
#define RACETRACK   4
#define COIN        5
#define ROCKET      6

// Váriaveis de áudio
extern ma_engine g_AudioEngine;

#endif //GLOBALS_H

