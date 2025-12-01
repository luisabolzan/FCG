

#ifndef ENEMYAI_H
#define ENEMYAI_H

#include "kart.h"
#include <vector>
#include <glm/glm.hpp>

class EnemyAI {
public:

    EnemyAI(Kart* enemyKart, Kart* targetKart);

    void SetupWaypoints();
    void UpdateAI();


private:
    Kart* enemy;
    Kart* target;
    std::vector<glm::vec3> waypoints;
    int currentWaypointIndex;


    float detectionRange; // Distância para começar a perseguir o player
    float stopDistance;   // Distância mínima dos waypoints para trocar para o próximo

    // Funções auxiliares de lógica
    void FollowPath();
    void ChaseTarget(glm::vec3 targetPos);
    void SteerTowards(glm::vec3 targetPos);
};

#endif //ENEMYAI_H
