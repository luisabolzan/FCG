#include "kart.h"
#include "audio.h"
#include "globals.h"
#include "matrices.h"
#include <cmath>
#include <algorithm> // Para std::abs

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
    isAlive(true),
    respawnTime(3.0f),
    respawnTimer(0.0f),
    invincibleTime(2.0f),
    invincibleTimer(0.0f),
    dummy(false),
    radius(1.1f),
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

// =========================================================================
// LOOP PRINCIPAL DO KART
// =========================================================================
void Kart::Update() {
    if (dummy) return;

    // Verifica se está morto
    if (!isAlive) {
        HandleRespawn();
        HandleAudio();
        return; // Não processa física nem tiro se estiver morto
    }

    // Lógicas de Estado
    HandleInvincibility();

    // Movimentação
    HandlePhysics();

    // Combate
    HandleShooting();

    // Áudio
    HandleAudio();
}

// =========================================================================
// MÉTODOS AUXILIARES
// =========================================================================

void Kart::HandleRespawn() {

    respawnTimer += deltaTime;
    if (respawnTimer >= respawnTime) {
        // Reinicia estado do jogador
        isAlive = true;
        isInvincible = true;
        health = 100;
        respawnTimer = 0.0f;
        position = spawnPosition;
        speed = 0.0f;
        rotation = glm::vec4(0.0f);
        direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    }
}

void Kart::HandleInvincibility() {

    if (isInvincible) {
        invincibleTimer += deltaTime;
        if (invincibleTimer >= invincibleTime) {
            isInvincible = false;
            invincibleTimer = 0.0f;
        }
    }
}

void Kart::HandlePhysics() {

    // Rotação
    if (inputLeft)
        rotation.y += turnSpeed * deltaTime;
    if (inputRight)
        rotation.y -= turnSpeed * deltaTime;

    // Calcula vetor direção baseado no ângulo Y
    direction.x = sin(rotation.y);
    direction.z = cos(rotation.y);
    direction = glm::normalize(direction);

    // Aceleração / Frenagem
    if (inputUp) {
        speed += acceleration * deltaTime;
        if (speed > maxSpeed)
            speed = maxSpeed;
    }
    else if (inputDown) {
        speed -= acceleration * deltaTime;
        // Ré é metade da maxSpeed
        if (speed < -maxSpeed / 2.0f)
            speed = -maxSpeed / 2.0f;
    }
    else {
        // Atrito / Inércia quando solta os botões
        if (speed > 0.0f) {
            speed -= acceleration * 0.5f * deltaTime;
            if (speed < 0.0f)
                speed = 0.0f;
        }
        else if (speed < 0.0f) {
            speed += acceleration * 0.5f * deltaTime;
            if (speed > 0.0f)
                speed = 0.0f;
        }
    }

    // Atualiza Posição
    position += direction * speed * deltaTime;
}

void Kart::HandleShooting() {

    // Tenta disparar
    if (inputFire) FireRocket();

    // Atualiza foguetes existentes
    for (auto& rocket : rockets) rocket.Update();
}

void Kart::FireRocket() {

    // Verifica Cooldown e Munição
    if (currentTime - lastShotTime < fireRate) return;
    if (ammo <= 0) return;

    // Calcula posição de spawn do míssil (um pouco à frente do kart)
    glm::vec4 spawnPos = position + direction * 1.3f + glm::vec4(0, 0.1f, 0, 0);

    rockets.emplace_back(spawnPos, direction, speed);
    
    lastShotTime = currentTime;
    ammo--;

    AudioShotSound();
}

void Kart::HandleAudio() {

    ma_sound* soundAcc = nullptr;
    ma_sound* soundDecel = nullptr;

    if (this->name == "Player1") {
        soundAcc = &g_SoundAccP1;
        soundDecel = &g_SoundDecelP1;
    } 
    else if (this->name == "Enemy" || this->name == "Player2") {
        soundAcc = &g_SoundAccP2;
        soundDecel = &g_SoundDecelP2;
    }

    if (!soundAcc || !soundDecel) return;

    // Se estiver morto, para tudo
    if (!isAlive) {
        if (ma_sound_is_playing(soundAcc)) ma_sound_stop(soundAcc);
        if (ma_sound_is_playing(soundDecel)) ma_sound_stop(soundDecel);
        return;
    }

    // Lógica de Pitch e Play/Stop
    bool isMoving = std::abs(speed) > 0.2f;
    bool isPressingGas = (inputUp || inputDown);
    float pitch = 0.8f + (std::abs(speed) / maxSpeed) * 0.6f;

    if (isMoving) {
        if (isPressingGas) {
            // Acelerando
            if (ma_sound_is_playing(soundDecel)) ma_sound_stop(soundDecel);
            if (!ma_sound_is_playing(soundAcc)) {
                ma_sound_set_volume(soundAcc, 0.5f);
                ma_sound_start(soundAcc);
            }
            ma_sound_set_pitch(soundAcc, pitch);
        } else {
            // Desacelerando (inércia)
            if (ma_sound_is_playing(soundAcc)) ma_sound_stop(soundAcc);
            if (!ma_sound_is_playing(soundDecel)) {
                ma_sound_set_volume(soundDecel, 0.5f);
                ma_sound_start(soundDecel);
            }
            ma_sound_set_pitch(soundDecel, pitch);
        }
    } else {
        // Parado
        if (ma_sound_is_playing(soundAcc)) ma_sound_stop(soundAcc);
        if (ma_sound_is_playing(soundDecel)) ma_sound_stop(soundDecel);
    }
}

// =========================================================================
// RENDERIZAÇÃO
// =========================================================================

void Kart::Render() {

    if (!isAlive) return;

    // Configura Iluminação
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_LAMBERT);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), true);

    // Matriz do Modelo
    glm::mat4 model = Matrix_Translate(position.x, position.y, position.z) 
                    * Matrix_Rotate_Y(rotation.y)
                    * Matrix_Scale(scale.x, scale.y, scale.z);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, KART);
    
    DrawVirtualObject("the_kart");

    // Renderiza Foguetes
    for (auto& rocket : rockets) {
        rocket.Render();
    }
}