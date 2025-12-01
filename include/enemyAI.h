#ifndef ENEMYAI_H
#define ENEMYAI_H

#include "kart.h"
#include <vector>
#include "coin.h"
#include <glm/glm.hpp>

// Comportamentos possíveis da IA
enum class AIState {
    PATROL,             // Segue os waypoints da pista
    CHASE,              // Persegue o jogador agressivamente
    FIND_AMMO,          // Procura a moeda mais próxima
    RECOVER             // Tenta sair de uma colisão (ré)
};

class EnemyAI {
public:

    EnemyAI(Kart* enemyKart, Kart* targetKart, std::vector<Coin>* coinsList);

    void SetupWaypoints();
    void UpdateAI();

private:
    // Referências
    Kart* enemy;
    Kart* target;
    std::vector<Coin>* coinsRef;

    // Navegação
    std::vector<glm::vec3> waypoints;
    int currentWaypointIndex;
    
    // Configurações de Comportamento
    float detectionRange;       // Raio para iniciar perseguição
    float stopDistance;         // Distância para aceitar que chegou no waypoint
    AIState currentState;       // Estado atual da máquina de estados

    // Variáveis para detectar se está preso (Stuck Detection)
    float stuckTimer;           // Cronômetro de tempo parado
    glm::vec3 lastPosition;     // Posição no último check
    float timeSinceLastCheck;   // Tempo desde o último check de posição

    void DecideState();

    void PerformPatrol();
    void PerformChase();
    void PerformSearchAmmo();
    void PerformRecovery();

    void SteerTowards(glm::vec3 targetPos, bool aggressive = false);
    void CheckIfStuck();
    glm::vec3 PredictTargetPos(Kart* target);
    glm::vec3 FindClosestCoin();
};

#endif //ENEMYAI_H