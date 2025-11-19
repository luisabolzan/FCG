
#ifndef ROCKET_H
#define ROCKET_H

#include "matrices.h"
#include "globals.h"
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

class Rocket {
public:
    glm::vec4 position;     // posição no mundo
    glm::vec4 prevPosition; // posição do último frame
    glm::vec4 direction;    // direção normalizada de movimento
    float rotationY;        // orientação fixa no momento do disparo
    float speed;            // velocidade constante
    bool active;            // se está ativo ou deve ser removido
    float lifetime;         // tempo total de vida
    float age;              // tempo desde o disparo

    Rocket(const glm::vec4& pos, const glm::vec4& dir, float kartSpeed);

    void UpdateMovement();
    void Render();

};

#endif //ROCKET_H
