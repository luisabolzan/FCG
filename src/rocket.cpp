#include "rocket.h"
#include <cmath>


Rocket::Rocket(const glm::vec4& pos, const glm::vec4& dir, float kartSpeed)
    : position(pos),
      prevPosition(pos),
      direction(glm::normalize(dir)),
      active(true),
      lifetime(5.0f),
      age(0.0f)
{
    speed = 20.0f + std::max(kartSpeed, 0.0f);  // herda velocidade do kart
    rotationY = atan2(direction.x, direction.z);
}


void Rocket::Update() {

    if (!active) return;
    prevPosition = position;
    age += deltaTime;

    if (age > lifetime) {
        active = false;
        return;
    }

    position += direction * speed * deltaTime;
}

void Rocket::Render() {

    if (!active) return;

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_PHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), true);

    glm::mat4 rocketModel = Matrix_Translate(position.x, position.y, position.z)
                          * Matrix_Rotate_Y(rotationY)  * Matrix_Scale(0.5f, 0.5f, 0.5f);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(rocketModel));
    glUniform1i(g_object_id_uniform, ROCKET);
    DrawVirtualObject("the_rocket");
}