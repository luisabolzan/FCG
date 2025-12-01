#include "enemyAI.h"
#include <cmath>
#include <iostream>

// Ajuste PI caso não esteja definido
#ifndef M_PI
#define M_PI 3.14159265359
#endif

EnemyAI::EnemyAI(Kart* enemyKart, Kart* targetKart)
    : enemy(enemyKart), target(targetKart), currentWaypointIndex(0)
{
    detectionRange = 25.0f; // Começa a perseguir se estiver a 25 metros
    stopDistance = 5.0f;    // Troca de waypoint se chegar a 5 metros dele
    SetupWaypoints();
}

void EnemyAI::SetupWaypoints() {

    waypoints.push_back(glm::vec3(0.0f, 0.0f, 25.0f));   // Reta 1 (início)
    waypoints.push_back(glm::vec3(20.0f, 0.0f, 25.0f));  // Entrada curva
    waypoints.push_back(glm::vec3(40.0f, 0.0f, 5.0f));   // Meio curva direita
    waypoints.push_back(glm::vec3(20.0f, 0.0f, -20.0f)); // Saída curva
    waypoints.push_back(glm::vec3(0.0f, 0.0f, -20.0f));  // Reta 2 (fundo)
    waypoints.push_back(glm::vec3(-20.0f, 0.0f, -20.0f));// Entrada curva esq
    waypoints.push_back(glm::vec3(-40.0f, 0.0f, 5.0f));  // Meio curva esq
    waypoints.push_back(glm::vec3(-20.0f, 0.0f, 25.0f)); // Volta pro início
}

void EnemyAI::UpdateAI() {

    if (!enemy->isAlive) {
        enemy->SetInputs(false, false, false, false, false);
        return;
    }

    float distToPlayer = glm::distance(glm::vec3(enemy->position), glm::vec3(target->position));

    if (distToPlayer < detectionRange && target->isAlive)
        ChaseTarget(glm::vec3(target->position));
    else
        FollowPath();
}

void EnemyAI::FollowPath() {

    if (waypoints.empty()) return;

    glm::vec3 targetPos = waypoints[currentWaypointIndex];
    float distToWaypoint = glm::distance(glm::vec3(enemy->position), targetPos);

    if (distToWaypoint < stopDistance) {
        currentWaypointIndex++;
        if (currentWaypointIndex >= waypoints.size()) {
            currentWaypointIndex = 0;
        }
    }

    SteerTowards(targetPos);
}

void EnemyAI::ChaseTarget(glm::vec3 targetPos) {
    SteerTowards(targetPos);

    glm::vec3 dirToTarget = glm::normalize(targetPos - glm::vec3(enemy->position));
    glm::vec3 enemyDir = glm::normalize(glm::vec3(enemy->direction));

    float alignment = glm::dot(dirToTarget, enemyDir);

    bool shouldFire = (alignment > 0.95f);

    if (shouldFire) {

        enemy->inputFire = true;
    }
}

void EnemyAI::SteerTowards(glm::vec3 targetPos) {

    glm::vec3 diff = targetPos - glm::vec3(enemy->position);

    float targetAngle = atan2(diff.x, diff.z);

    float currentAngle = enemy->rotation.y;

    float angleDiff = targetAngle - currentAngle;

    while (angleDiff > M_PI)  angleDiff -= 2.0f * M_PI;
    while (angleDiff < -M_PI) angleDiff += 2.0f * M_PI;

    bool up = true;
    bool down = false;
    bool left = false;
    bool right = false;
    bool fire = false;


    if (angleDiff > 0.1f) {
        left = true;
    } else if (angleDiff < -0.1f) {
        right = true;
    }

    if (std::abs(angleDiff) > 1.0f) {
        up = false;
    }

    enemy->SetInputs(up, down, left, right, fire);
}
