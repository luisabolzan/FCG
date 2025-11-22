
#include "kart.h"



Kart::Kart(const std::string& name, const ObjModel &obj, const glm::vec4& startPos):
    name(name),
    kartModel(obj),
    position(startPos),
    spawnPosition(startPos),
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
    respawnTime(3.0f),
    respawnTimer(0.0f),
    invincibleTime(2.0f),
    invincibleTimer(0.0f),
    dummy(false),
    radius(1.1f),
    isColliding(false),
    inputUp(false),
    inputDown(false),
    inputLeft(false),
    inputRight(false),
    inputFire(false)
{
    direction = glm::normalize(direction);
}


void Kart::SetInputs(bool up, bool down, bool left, bool right, bool fire) {
    this->inputUp = up;
    this->inputDown = down;
    this->inputLeft = left;
    this->inputRight = right;
    this->inputFire = fire;
}


void Kart::UpdateMovement() {

    if (dummy)
        return;

    if (inputLeft)
        rotation.y += turnSpeed * deltaTime;
    if (inputRight)
        rotation.y -= turnSpeed * deltaTime;

    direction.x = sin(rotation.y);
    direction.z = cos(rotation.y);
    direction = glm::normalize(direction);

    if (inputUp) {
        speed += acceleration * deltaTime;
        if (speed > maxSpeed)
            speed = maxSpeed;
    }
    else if (inputDown) {
        speed -= acceleration * deltaTime;
        if (speed < -maxSpeed / 2.0f)
            speed = -maxSpeed / 2.0f;
    }
    else {
        if (speed > 0.0f) {
            speed -= acceleration * 0.5f * deltaTime;
            if (speed < 0.0f) speed = 0.0f;
        }
        else if (speed < 0.0f) {
            speed += acceleration * 0.5f * deltaTime;
            if (speed > 0.0f) speed = 0.0f;
        }
    }

    position += direction * speed * deltaTime;
}

void Kart::FireRocket() {
    if (dummy)
        return;
    if (currentTime - lastShotTime < fireRate)
        return;
    if (ammo <= 0)
        return;

    glm::vec4 spawnPos = position + direction * 1.3f + glm::vec4(0, 0.1f, 0, 0);

    rockets.emplace_back(spawnPos, direction, speed);
    lastShotTime = currentTime;
    ammo--;
}

void Kart::Render() {

    if (!isAlive) {
        respawnTimer += deltaTime;
        if (respawnTimer >= respawnTime) {
            isAlive = true;
            isInvincible = true;       // revive INVENCÍVEL
            respawnTimer = 0.0f;
            position = position; // spawnPosition; você precisa guardar posição inicial
        }
        return;
    }

    this->UpdateMovement();

    if (isInvincible) {
        invincibleTimer += deltaTime;
        if (invincibleTimer >= invincibleTime) {
            isInvincible = false;
            invincibleTimer = 0.0f;
        }
    }

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_LAMBERT);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), true);

    glm::mat4 model = Matrix_Translate(position.x, position.y, position.z) * Matrix_Rotate_Y(rotation.y);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, KART);
    DrawVirtualObject("the_kart");

    if (inputFire)
        this->FireRocket();

    for (auto& rocket : rockets) {
        rocket.Render();
    }
}





