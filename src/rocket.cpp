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