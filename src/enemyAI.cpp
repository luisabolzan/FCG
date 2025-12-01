#include "enemyAI.h"
#include "globals.h"
#include <cmath>
#include <iostream>
#include <algorithm>


EnemyAI::EnemyAI(Kart* enemyKart, Kart* targetKart, std::vector<Coin>* coinsList):
    enemy(enemyKart),
    target(targetKart),
    coinsRef(coinsList),
    currentWaypointIndex(0),
    currentState(AIState::PATROL),
    stuckTimer(0.0f),
    timeSinceLastCheck(0.0f)
{
    detectionRange = 35.0f;
    stopDistance = 8.0f;
    lastPosition = enemy->position;
    SetupWaypoints();
}

void EnemyAI::SetupWaypoints() {

    // Waypoints ajustados para o centro da pista (aproximado)
    waypoints.push_back(glm::vec3(0.0f, 0.0f, 25.0f));
    waypoints.push_back(glm::vec3(20.0f, 0.0f, 25.0f));
    waypoints.push_back(glm::vec3(40.0f, 0.0f, 5.0f));
    waypoints.push_back(glm::vec3(20.0f, 0.0f, -20.0f));
    waypoints.push_back(glm::vec3(0.0f, 0.0f, -20.0f));
    waypoints.push_back(glm::vec3(-20.0f, 0.0f, -20.0f));
    waypoints.push_back(glm::vec3(-40.0f, 0.0f, 5.0f));
    waypoints.push_back(glm::vec3(-20.0f, 0.0f, 25.0f));
}

void EnemyAI::UpdateAI() {

    if (!enemy->isAlive) {
        enemy->SetInputs(false, false, false, false, false);
        return;
    }

    CheckIfStuck();
    DecideState();

    switch (currentState) {
        case AIState::RECOVER:
            PerformRecovery();
            break;
        case AIState::FIND_AMMO:
            PerformSearchAmmo();
            break;
        case AIState::CHASE:
            PerformChase();
            break;
        case AIState::PATROL:
        default:
            PerformPatrol();
            break;
    }
}

void EnemyAI::CheckIfStuck() {

    if (currentState == AIState::RECOVER) {
        stuckTimer -= deltaTime;
        if (stuckTimer <= 0.0f)
            currentState = AIState::PATROL;
        return;
    }

    timeSinceLastCheck += deltaTime;
    if (timeSinceLastCheck > 0.5f) {
        float distMoved = glm::distance(glm::vec3(enemy->position), lastPosition);

        if (distMoved < 0.5f && std::abs(enemy->speed) < 1.0f) {
            currentState = AIState::RECOVER;
            stuckTimer = 1.5f;
        }

        lastPosition = enemy->position;
        timeSinceLastCheck = 0.0f;
    }
}

void EnemyAI::DecideState() {

    if (currentState == AIState::RECOVER)
        return;

    if (enemy->ammo <= 0) {
        currentState = AIState::FIND_AMMO;
        return;
    }

    float distToPlayer = distance(glm::vec3(enemy->position), glm::vec3(target->position));

    if (distToPlayer < detectionRange && target->isAlive)
        currentState = AIState::CHASE;
    else
        currentState = AIState::PATROL;
}

void EnemyAI::PerformSearchAmmo() {

    glm::vec3 closestCoinPos = FindClosestCoin();


    if (closestCoinPos == glm::vec3(0.0f)) {
        PerformPatrol();
        return;
    }

    SteerTowards(closestCoinPos, false);
}

glm::vec3 EnemyAI::FindClosestCoin() {

    float minDistance = std::numeric_limits<float>::max();
    glm::vec3 bestPos(0.0f);
    bool found = false;

    for (const auto& coin : *coinsRef) {

        if (!coin.active) continue;

        float dist = distance(glm::vec3(enemy->position), glm::vec3(coin.position));
        if (dist < minDistance) {
            minDistance = dist;
            bestPos = glm::vec3(coin.position);
            found = true;
        }
    }

    if (!found) return glm::vec3(0.0f); // Código de erro: nenhuma moeda
    return bestPos;
}

void EnemyAI::PerformRecovery() {
    enemy->SetInputs(false, true, true, false, false);
}

void EnemyAI::PerformPatrol() {

    if (waypoints.empty()) return;

    glm::vec3 targetPos = waypoints[currentWaypointIndex];
    float distToWaypoint = glm::distance(glm::vec3(enemy->position), targetPos);

    if (distToWaypoint < stopDistance) {
        currentWaypointIndex++;
        if (currentWaypointIndex >= waypoints.size()) {
            currentWaypointIndex = 0;
        }
    }

    SteerTowards(targetPos, false);
}

void EnemyAI::PerformChase() {

    glm::vec3 futurePos = PredictTargetPos(target);
    SteerTowards(futurePos, true);
}

glm::vec3 EnemyAI::PredictTargetPos(Kart* target) {

    float dist = distance(glm::vec3(enemy->position), glm::vec3(target->position));
    float rocketSpeed = enemy->speed + 40.0f;
    float timeToHit = dist / rocketSpeed;

    if (timeToHit > 1.5f) timeToHit = 1.5f;

    return glm::vec3(target->position) + (glm::vec3(target->direction) * target->speed * timeToHit);
}

void EnemyAI::SteerTowards(glm::vec3 targetPos, bool aggressive) {

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

    if (angleDiff > 0.15f)
        left = true;
    else if (angleDiff < -0.15f)
        right = true;


    if (std::abs(angleDiff) > 0.8f)
        up = false;
    if (std::abs(angleDiff) > 1.5f)
        down = true;


    if (aggressive) {
        if (std::abs(angleDiff) < 0.2f)
            fire = true;
    }

    enemy->SetInputs(up, down, left, right, fire);
}