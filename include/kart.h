
#ifndef KART_H
#define KART_H
#include <string>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "globals.h"
#include "matrices.h"
#include "FCGstructures.h"
#include "rocket.h"


class Kart {

    public:

    // Atributos básicos:
    std::string name;           // nome do jogador ou do kart
    ObjModel kartModel;         // arquivo Obj
    glm::vec4 position;         // posição no mundo
    glm::vec4 rotation;         // rotação em torno dos eixos
    glm::vec4 scale;            // escala do modelo

    // Atributos de movimento
    float speed;                // velocidade atual
    float acceleration;         // aceleração do kart
    float maxSpeed;             // velocidade máxima
    float turnSpeed;            // velocidade de rotação
    glm::vec4 direction;        // direção atual (vetor normalizado)

    // Atributos de combate
    int health;                     // pontos de vida
    int ammo;                       // munição disponível
    std::vector<Rocket> rockets;    // foguetes disparados
    float fireRate;                 // tempo mínimo entre disparos
    float lastShotTime;             // controle de cooldown de tiro
    bool isInvincible;              // invulnerabilidade temporária

    // Atributos de jogo
    int score;                  // pontuação acumulada
    int coins;                  // número de moedas coletadas
    bool isAlive;               // se está ativo no jogo

    // Atributos de física/colisão
    float radius;               // raio da colisão
    bool isColliding;           // flag de colisão

    // Estados e controle
    bool accelerating;          // está acelerando?
    bool braking;               // está freando?
    bool turningLeft;           // virando à esquerda
    bool turningRight;          // virando à direita


    Kart(const std::string& name, ObjModel obj, const glm::vec4& startPos);

    void UpdateMovement();
    void FireRocket();
    void Render();
};

#endif //KART_H
