#include "camera.h"

float g_CameraTheta = 0.0f;       // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;         // Ângulo em relação ao eixo Y
float g_CameraDistance = 3.5f;    // Distância da câmera para a origem

float nearplane = -0.1f;  // Posição do "near plane"
float farplane  = -300.0f; // Posição do "far plane"
float field_of_view = 3.141592 / 3.0f;

Camera::Camera() {

    g_FreeCamera = false;

    // Computamos a posição da câmera utilizando coordenadas esféricas.
    float r = g_CameraDistance;
    float y = r*sin(g_CameraPhi);
    float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
    position_c = glm::vec4(x,y,z,1.0f);;

    lookat = glm::vec4(0.0f,0.0f,0.0f,1.0f);;

    if (!g_FreeCamera){
        view_vector = lookat - position_c;
    }
    else {
        view_vector = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    }

    up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    view_matrix = Matrix_Camera_View(position_c, view_vector, up_vector);
    projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);


}

glm::vec3 Camera::GetPosition(){
    return glm::vec3(position_c.x, position_c.y, position_c.z);
}

float Camera::GetPositionX() {
    return position_c.x;
}

float Camera::GetPositionY() {
    return position_c.y;
}

float Camera::GetPositionZ() {
    return position_c.z;
}

glm::mat4 Camera::GetViewMatrix(){
    return view_matrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
    return projection;
}

glm::vec4 Camera::GetViewVector(){
    return view_vector;
}

