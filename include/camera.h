
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "matrices.h"
#include "globals.h"


extern float g_CameraTheta;       // Ângulo no plano ZX em relação ao eixo Z
extern float g_CameraPhi;         // Ângulo em relação ao eixo Y
extern float g_CameraDistance;    // Distância da câmera para a origem

extern float nearplane;           // Posição do "near plane"
extern float farplane;            // Posição do "far plane"
extern float field_of_view;

class Camera {

    private:

    bool g_FreeCamera;              // booleano para saber se é camera livre
    glm::vec4 position_c;           // Ponto "c", centro da câmera
    glm::vec4 lookat;               // Ponto para onde a câmera (look-at) estará sempre olhando
    glm::vec4 view_vector;          // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 up_vector;            // Vetor "up" fixado para apontar para o "céu" (eito Y global)
    glm::mat4 view_matrix;          // Matriz view
    glm::mat4 projection;           // Matriz de Projeção

    public:

    //Camera(glm::vec3 StartingPosition);
    Camera();

    glm::vec3 GetPosition();
    float GetPositionX();
    float GetPositionY();
    float GetPositionZ();

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec4 GetViewVector();

};


#endif //CAMERA_H
