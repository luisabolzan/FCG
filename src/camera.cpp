#include "camera.h"

float g_CameraTheta  = 0.0f;       // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = -0.5f;         // Ângulo em relação ao eixo Y
float g_CameraDistance = 7.0f;    // Distância da câmera para a origem

float nearplane = -0.1f;  // Posição do "near plane"
float farplane  = -300.0f; // Posição do "far plane"
float field_of_view = 3.141592 / 3.0f;

Camera::Camera() {

    FreeCamera = false;

    position = glm::vec4(0.0f, 0.0f, 0.0f, 1);
    up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    view_vector = glm::vec4(0,0,1,0);
    view_matrix = Matrix_Camera_View(position, view_vector, up_vector);

    projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);

    c_theta = g_CameraTheta;
    c_phi = g_CameraPhi;
    c_distance = g_CameraDistance;

    U = crossproduct(view_vector, up_vector);

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
}

void Camera::UpdateFreeCamera() {
    float cosPhi = cos(g_CameraPhi);
    float sinPhi = sin(g_CameraPhi);
    float cosTheta = cos(g_CameraTheta);
    float sinTheta = sin(g_CameraTheta);

    view_vector = glm::vec4(cosPhi * sinTheta, sinPhi, cosPhi * cosTheta, 0.0f);

    U = normalize(crossproduct(view_vector, up_vector));

    view_matrix = Matrix_Camera_View(position, view_vector, up_vector);
}

void Camera::UpdateLookAtCamera(const Kart& kart) {

    float r = c_distance;

    // Ângulos locais
    const float cosPhi = cos(c_phi);
    const float sinPhi = sin(c_phi);

    // O Theta da câmera é somado à rotação do Kart para que ela gire junto com ele
    const float cosTheta = cos(c_theta + kart.rotation.y);
    const float sinTheta = sin(c_theta + kart.rotation.y);

    glm::vec4 offset = glm::vec4(
        r * cosPhi * sinTheta,
        r * sinPhi,
        r * cosPhi * cosTheta,
        0.0f
    );

    glm::vec4 kartCenter = kart.position + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    position = kart.position - offset;

    lookat = kartCenter;

    view_vector = glm::normalize(lookat - position);
    view_matrix = Matrix_Camera_View(position, view_vector, up_vector);
}

void Camera::SyncVectorToAngles() {

    glm::vec3 v = glm::normalize(glm::vec3(view_vector));
    g_CameraPhi   = asin(v.y);
    g_CameraTheta = atan2(v.x, v.z);
}

void Camera::StartCamera(const Kart& kart) {

    if (CPressed) {
        this->SetFreeCamera(!this->GetFreeCamera());
        this->SyncVectorToAngles();
        CPressed = false;
    }

    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (this->GetFreeCamera()) {

        float speed = this->GetSpeed() * deltaTime;
        if (WPressed){
            this->SetPosition(this->GetPosition() + this->GetViewVector() * speed);
        }
        if (APressed){
            this->SetPosition(this->GetPosition() - this->GetU() * speed);
        }
        if (SPressed){
            this->SetPosition(this->GetPosition() - this->GetViewVector() * speed);
        }
        if (DPressed){
            this->SetPosition(this->GetPosition() + this->GetU() * speed);
        }
        if (SpacePressed) {
            this->SetPositionY(this->GetPositionY() + speed);
        }
        if (LeftShiftPressed) {
            this->SetPositionY(this->GetPositionY() - speed);
        }
        this->UpdateFreeCamera();
    }
    else
        this->UpdateLookAtCamera(kart);

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo (GPU).
    glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(this->GetViewMatrix()));
    glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(this->GetProjectionMatrix()));
}

void Camera::UpdateProjectionMatrix(float screenRatio) {
    projection = Matrix_Perspective(field_of_view, screenRatio, nearplane, farplane);
}




