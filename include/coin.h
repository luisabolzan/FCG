
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

    glm::vec3 p0, p1, p2, p3;
    Coin(glm::vec4 pos, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3);

    void Update(float time);
    void Render(float terraineHeight);
};

#endif //COIN_H
