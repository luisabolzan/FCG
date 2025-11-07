#include "camera.h"

float g_CameraTheta = 0.0f;       // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;         // Ângulo em relação ao eixo Y
float g_CameraDistance = 3.5f;    // Distância da câmera para a origem

Camera::Camera(glm::vec3 StartingPosition) {

    g_FreeCamera = false;
    position_c = glm::vec4(StartingPosition.x, StartingPosition.y, StartingPosition.z, 1.0f);;
    lookat = glm::vec4(0.0f,0.0f,0.0f,1.0f);;

    if (!g_FreeCamera){
        view_vector = lookat - position_c;
    }
    else {
        view_vector = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    }

    up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    view_matrix = Matrix_Camera_View(position_c, view_vector, up_vector);;

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
    return Matrix_Camera_View(position_c, view_vector, up_vector);
}

glm::vec4 Camera::GetViewVector(){
    return view_vector;
}

