
#ifndef GLOBALS_H
#define GLOBALS_H

#include <map>
#include <stack>
#include <glad/glad.h>

#include "FCGfunctions.h"

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

extern float currentTime;
extern float deltaTime;
extern float lastTime;

extern bool WPressed;
extern bool APressed;
extern bool SPressed;
extern bool DPressed;

extern bool CPressed;

extern bool SpacePressed;
extern bool ShiftPressed;

#endif //GLOBALS_H
