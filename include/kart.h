#ifndef KART_H
#define KART_H

#include <string>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "globals.h"
#include "FCGstructures.h"
#include "rocket.h"

class Kart {

public:

    // --- Identificação e Modelo ---
    std::string name;
    ObjModel kartModel;

    // --- Estado Espacial ---
    glm::vec4 position;
    glm::vec4 spawnPosition;
    glm::vec4 rotation;
    glm::vec4 scale;

    // --- Física e Movimento ---
    float speed;
    float acceleration;
    float maxSpeed;
    float turnSpeed;
    glm::vec4 direction;

    // --- Gameplay e Combate ---
    int health;
    int ammo;
    int score;
    bool isAlive;
    bool isInvincible;
    bool dummy;
    float radius;

    // --- Inputs ---
    bool inputUp;
    bool inputDown;
    bool inputLeft;
    bool inputRight;
    bool inputFire;

    // --- Temporizadores ---
    float fireRate;
    float lastShotTime;
    float respawnTime;
    float respawnTimer;
    float invincibleTime;
    float invincibleTimer;

    // --- Container de Projéteis ---
    std::vector<Rocket> rockets;

    // Construtor
    Kart(const std::string& name, const ObjModel &obj, const glm::vec4& startPos);

    // Métodos Públicos
    void SetInputs(bool up, bool down, bool left, bool right, bool fire);
    void Update();
    void Render();

private:

    // Métodos Auxiliares
    void HandleRespawn();       // Gerencia morte e renascimento
    void HandleInvincibility(); // Gerencia tempo de invencibilidade
    void HandlePhysics();       // Gerencia aceleração, rotação e posição
    void HandleShooting();      // Gerencia disparo e atualização de foguetes
    void HandleAudio();         // Gerencia sons de motor
    void FireRocket();          // Lógica interna de criar o foguete
};

#endif //KART_H