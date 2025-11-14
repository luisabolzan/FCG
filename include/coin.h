
#ifndef COIN_H
#define COIN_H

#include "matrices.h"
#include "globals.h"
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>


class Coin {
public:
    glm::vec4 position;     // posição no mundo
    bool active;            // se está visível
    float respawnTime;      // tempo para reaparecer
    float respawnTimer;     // contador do respawn
    float rotationY;

    Coin(glm::vec4 pos);

    void Render(const glm::vec4 pos);
};

#endif //COIN_H
