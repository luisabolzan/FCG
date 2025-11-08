
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

    bool FreeCamera;                // booleano para saber se é camera livre
    glm::vec4 position;             // Ponto "c", centro da camera
    glm::vec4 lookat;               // Ponto para onde a câmera (look-at) estará sempre olhando
    glm::vec4 view_vector;          // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 up_vector;            // Vetor "up" fixado para apontar para o "céu" (eito Y global)
    glm::mat4 view_matrix;          // Matriz view
    glm::mat4 projection;           // Matriz de Projeção

    glm::vec4 U;


    float r;

    float speed;                    // Velocidade da Camera

    public:

    //Camera(glm::vec3 StartingPosition);
    Camera();

    glm::vec4 GetPosition();
    float GetPositionX();
    float GetPositionY();
    float GetPositionZ();

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec4 GetViewVector();
    bool GetFreeCamera();
    float GetSpeed();
    glm::vec4 GetU();

    void SetFreeCamera(bool freeCamera);
    void SetPosition(glm::vec4 position);
    void SetPositionY(float y);

    void SetLookAt(glm::vec4 lookat);

    void UpdateFreeCamera();
    void UpdateLookAtCamera();
    void SyncVectorToAngles();

};


#endif //CAMERA_H
