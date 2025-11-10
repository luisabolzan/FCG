
#include "kart.h"


Kart::Kart(const std::string& name, ObjModel obj, const glm::vec4& startPos):
    name(name),
    kartModel(obj),
    position(startPos),
    rotation(0.0f),
    scale(1.0f, 1.0f, 1.0f, 1.0f),
    speed(0.0f),
    acceleration(10.0f),
    maxSpeed(25.0f),
    turnSpeed(3.0f),
    direction(0.0f, 0.0f, -1.0f, 0.0f),
    health(100),
    ammo(10),
    fireRate(0.5f),
    lastShotTime(0.0f),
    isInvincible(false),
    score(0),
    coins(0),
    isAlive(true),
    radius(1.0f),
    isColliding(false),
    accelerating(false),
    braking(false),
    turningLeft(false),
    turningRight(false)
{
    direction = glm::normalize(direction);
}

void Kart::UpdateMovement()
{

    if (APressed)
        rotation.y += turnSpeed * deltaTime;
    if (DPressed)
        rotation.y -= turnSpeed * deltaTime;

    direction.x = sin(rotation.y);
    direction.z = cos(rotation.y);
    direction = glm::normalize(direction);

    if (WPressed){
        speed += acceleration * deltaTime;
        if (speed > maxSpeed)
            speed = maxSpeed;
    }
    else if (SPressed){
        speed -= acceleration * deltaTime;
        if (speed < -maxSpeed / 2.0f)
            speed = -maxSpeed / 2.0f;
    }
    else{

        if (speed > 0.0f)
        {
            speed -= acceleration * 0.5f * deltaTime;
            if (speed < 0.0f)
                speed = 0.0f;
        }
        else if (speed < 0.0f)
        {
            speed += acceleration * 0.5f * deltaTime;
            if (speed > 0.0f)
                speed = 0.0f;
        }
    }

    position += direction * speed * deltaTime;
}

