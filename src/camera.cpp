#include "camera.h"

float g_CameraTheta  = 0.0f;       // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;         // Ângulo em relação ao eixo Y
float g_CameraDistance = 3.5f;    // Distância da câmera para a origem

float nearplane = -0.1f;  // Posição do "near plane"
float farplane  = -300.0f; // Posição do "far plane"
float field_of_view = 3.141592 / 3.0f;

Camera::Camera() {

    FreeCamera = false;

    position = glm::vec4(0, 1.8f, 5, 1);
    up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    view_vector = glm::vec4(0,0,1,0);
    view_matrix = Matrix_Camera_View(position, view_vector, up_vector);

    projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);

    U = crossproduct(view_vector, up_vector);
    W = -view_vector / norm(view_vector);

    speed = 10.0f;
}

glm::vec4 Camera::GetPosition(){
    return position;
}

float Camera::GetPositionX() {
    return position.x;
}

float Camera::GetPositionY() {
    return position.y;
}

float Camera::GetPositionZ() {
    return position.z;
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

bool Camera::GetFreeCamera() {
    return FreeCamera;
}

float Camera::GetSpeed() {
    return speed;
}

glm::vec4 Camera::GetU() {
    return U;
}


void Camera::SetFreeCamera(bool freeCamera) {
    this->FreeCamera = freeCamera;
}

void Camera::SetPosition(glm::vec4 position) {
    this->position = position;
}

void Camera::SetPositionY(float y) {
    this->position.y = y;
}

void Camera::SetLookAt(glm::vec4 lookat) {
    this->lookat = lookat;
    this->view_vector = lookat - position;
}

void Camera::UpdateFreeCamera() {
    float cosPhi = cos(g_CameraPhi);
    float sinPhi = sin(g_CameraPhi);
    float cosTheta = cos(g_CameraTheta);
    float sinTheta = sin(g_CameraTheta);

    view_vector = glm::vec4( cosPhi * sinTheta, sinPhi, cosPhi * cosTheta, 0.0f);

    U = normalize( crossproduct(view_vector, up_vector) );
    W = -normalize(view_vector);

    view_matrix = Matrix_Camera_View(position, view_vector, up_vector);
}

void Camera::UpdateLookAtCamera() {
    float r = g_CameraDistance;
    float y = -r*sin(g_CameraPhi);
    float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
    position = glm::vec4(x,y,z,1.0f);

    lookat = glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
    view_vector = glm::normalize(lookat - position);

    view_matrix = Matrix_Camera_View(position, view_vector, up_vector);
}

void Camera::SyncVectorToAngles() {

    glm::vec3 v = glm::normalize(glm::vec3(view_vector));
    g_CameraPhi   = asin(v.y);
    g_CameraTheta = atan2(v.x, v.z);
}



