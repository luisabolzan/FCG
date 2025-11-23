#include <algorithm>
#include "collisions.h"
#include "kart.h"
#include "coin.h"
#include "scene.h"
#include "audio.h"

// Colisão Kart (sphere) com Kart (sphere)
bool CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b) {
    float distSq = glm::dot(a.center - b.center, a.center - b.center);
    float radiusSum = a.radius + b.radius;
    return distSq <= radiusSum * radiusSum;
}

// Colisão Coin (ponto) com Kart (sphere)
bool CheckPointSphere(const glm::vec3& point, const BoundingSphere& sphere){
    float distSq = glm::dot(point - sphere.center, point - sphere.center);
    return distSq <= sphere.radius * sphere.radius;
}

// Colisão Kart (sphere) com Cenário (Cube)
bool CheckSphereCube(const BoundingSphere& sphere, const AABB& box) {
    glm::vec3 closestPoint = glm::clamp(sphere.center, box.min, box.max);
    float distSq = glm::dot(sphere.center - closestPoint, sphere.center - closestPoint);
    return distSq <= sphere.radius * sphere.radius;
}

// Colisão rocket (ray) com Cenário (Cube)
bool CheckRayCube(const glm::vec3& origin, const glm::vec3& dir, const AABB& box, float& t) {
    glm::vec3 invDir;
    invDir.x = (std::abs(dir.x) < 1e-6f) ? 1e20f : 1.0f / dir.x;
    invDir.y = (std::abs(dir.y) < 1e-6f) ? 1e20f : 1.0f / dir.y;
    invDir.z = (std::abs(dir.z) < 1e-6f) ? 1e20f : 1.0f / dir.z;

    glm::vec3 t1 = (box.min - origin) * invDir;
    glm::vec3 t2 = (box.max - origin) * invDir;

    glm::vec3 tmin = glm::min(t1, t2);
    glm::vec3 tmax = glm::max(t1, t2);

    float tNear = std::max({tmin.x, tmin.y, tmin.z});
    float tFar  = std::min({tmax.x, tmax.y, tmax.z});

    if (tNear > tFar || tFar < 0.0f)
        return false;

    if (tNear < 0.0f) {
        t = 0.0f;
    } else {
        t = tNear;
    }

    return true;
}

// Colisão rocket (ray) com Kart (sphere)
bool CheckRaySphere(const glm::vec3& origin, const glm::vec3& dir, const BoundingSphere& sphere, float& t) {

    glm::vec3 oc = origin - sphere.center;

    float a = glm::dot(dir, dir);
    float b = 2.0f * glm::dot(oc, dir);
    float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f)
        return false;

    float sqrtDisc = sqrt(discriminant);
    float t1 = (-b - sqrtDisc) / (2.0f * a);
    float t2 = (-b + sqrtDisc) / (2.0f * a);

    if (t1 >= 0.0f) {
        t = t1;
        return true;
    }
    if (t2 >= 0.0f) {
        t = t2;
        return true;
    }

    return false;
}


void HandleCollisions(Scene& scene) {

    // Verifica se os foguetes acertaram algo
    CheckRocketKartCollision(scene.player1, scene.player2);
    CheckRocketKartCollision(scene.player2, scene.player1);
    CheckRocketPalmCollision(scene.player1, scene.palmPositions);
    CheckRocketTreeCollision(scene.player1, scene.treePositions);
    CheckRocketPalmCollision(scene.player2, scene.palmPositions);
    CheckRocketTreeCollision(scene.player2, scene.treePositions);

    // Verifica Players contra as moedas
    for (auto& c : scene.coins) {
        CheckKartCoinCollision(scene.player1, c);
        CheckKartCoinCollision(scene.player2, c);
    }

    // Verifica Player contra Player
    CheckKartKartCollision(scene.player1, scene.player2);

    // Verifica Players contra o ambiente
    CheckKartPalmCollision(scene.player1, scene.palmPositions);
    CheckKartTreeCollision(scene.player1, scene.treePositions);
    CheckKartPalmCollision(scene.player2, scene.palmPositions);
    CheckKartTreeCollision(scene.player2, scene.treePositions);
}


void CheckRocketKartCollision(Kart& shooter, Kart& target) {
    for (auto& rocket : shooter.rockets) {

        if (!rocket.active) continue;
        if (!target.isAlive) continue;
        if (target.isInvincible) continue;

        glm::vec3 segmentStart = glm::vec3(rocket.prevPosition);
        glm::vec3 segmentDirection = normalize(glm::vec3(rocket.position - rocket.prevPosition));
        float segmentLength = length(glm::vec3(rocket.position - rocket.prevPosition));

        if (segmentLength <= 0.0f) continue;

        float hitDistance;
        BoundingSphere targetSphere;
        targetSphere.center = glm::vec3(target.position);
        targetSphere.radius = target.radius;

        if (CheckRaySphere(segmentStart, segmentDirection, targetSphere, hitDistance) &&
            hitDistance >= 0.0f && hitDistance <= segmentLength)
        {
            rocket.active = false;
            target.isAlive = false;
            AudioExplosionSound();
            printf(" %s atingiu %s!\n", shooter.name.c_str(), target.name.c_str());
        }
    }
}

void CheckKartCoinCollision(Kart& kart, Coin& coin) {

    if (!coin.active) return;

    BoundingSphere kartSphere{};
    kartSphere.center = glm::vec3(kart.position);
    kartSphere.radius = kart.radius;

    glm::vec3 coinPoint = glm::vec3(coin.position);

    if (CheckPointSphere(coinPoint, kartSphere)) {

        coin.active = false;
        coin.respawnTimer = 0.0f;
        kart.ammo++;
        AudioCoinSound();
        printf("%s pegou uma moeda!\n", kart.name.c_str());
    }
}

void CheckKartKartCollision(Kart& kartA, Kart& kartB) {

    if (!kartA.isAlive || !kartB.isAlive) return;

    BoundingSphere sphereA{};
    sphereA.center = glm::vec3(kartA.position);
    sphereA.radius = kartA.radius;

    BoundingSphere sphereB{};
    sphereB.center = glm::vec3(kartB.position);
    sphereB.radius = kartB.radius;

    if (CheckSphereSphere(sphereA, sphereB)) {

        glm::vec3 vectorBetweenCenters = sphereA.center - sphereB.center;
        float currentDistance = length(vectorBetweenCenters);

        float combinedRadii = sphereA.radius + sphereB.radius;
        float penetrationDepth = combinedRadii - currentDistance;

        glm::vec3 collisionNormal;
        if (currentDistance == 0.0f) {
            collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        } else {
            collisionNormal = normalize(vectorBetweenCenters);
        }

        glm::vec4 correctionVector = glm::vec4(collisionNormal * (penetrationDepth / 2.0f), 0.0f);

        kartA.position += correctionVector;
        kartB.position -= correctionVector;
    }
}

// Função auxiliar para resolver a colisão física
void ResolveSphereCubeCollision(Kart& kart, const BoundingSphere& sphere, const AABB& box) {

    glm::vec3 closestPoint = glm::clamp(sphere.center, box.min, box.max);

    glm::vec3 diff = sphere.center - closestPoint;
    float distSq = glm::dot(diff, diff);
    float dist = std::sqrt(distSq);


    float penetration = sphere.radius - dist;

    if (penetration > 0.0f) {
        glm::vec4 correction = glm::vec4(normalize(diff) * penetration, 0.0f);
        kart.position += correction;
    }
}

void CheckKartPalmCollision(Kart& kart, const std::vector<glm::vec3>& positions) {

    BoundingSphere kartSphere{};
    kartSphere.center = glm::vec3(kart.position);
    kartSphere.radius = kart.radius;

    // Tamanho da caixa de colisão da palmeira
    glm::vec3 halfSize(0.1f, 5.0f, 0.1f);

    for (const auto& pos : positions) {
        AABB box{};
        box.min = pos - halfSize;
        box.max = pos + halfSize;

        if (CheckSphereCube(kartSphere, box)) {
            ResolveSphereCubeCollision(kart, kartSphere, box);
            kart.speed = 0.0f;
            // printf("Batida na Palmeira!\n");
        }
    }
}

void CheckKartTreeCollision(Kart& kart, const std::vector<glm::vec3>& positions) {

    BoundingSphere kartSphere{};
    kartSphere.center = glm::vec3(kart.position);
    kartSphere.radius = kart.radius;

    // Tamanho da caixa de colisão da árvore
    glm::vec3 halfSize(0.1f, 10.0f, 0.1f);

    for (const auto& pos : positions) {
        AABB box{};
        glm::vec3 collisionPos = pos;
        collisionPos.y = 0.0f;
        box.min = collisionPos - halfSize;
        box.max = collisionPos + halfSize;

        if (CheckSphereCube(kartSphere, box)) {
            ResolveSphereCubeCollision(kart, kartSphere, box);
            kart.speed = 0.0f;
            // printf("Batida na Arvore!\n");
        }
    }
}


void CheckRocketPalmCollision(Kart& shooter, const std::vector<glm::vec3>& positions) {

    // Tamanho da caixa de colisão da palmeira
    glm::vec3 halfSize(0.5f, 5.0f, 0.5f);

    for (auto& rocket : shooter.rockets) {
        if (!rocket.active) continue;

        glm::vec3 origin = glm::vec3(rocket.prevPosition);
        glm::vec3 diff = glm::vec3(rocket.position - rocket.prevPosition);
        float segmentLength = length(diff);
        if (segmentLength <= 0.0f) continue;
        glm::vec3 dir = normalize(diff);

        for (const auto& pos : positions) {
            AABB box{};
            box.min = pos - halfSize;
            box.max = pos + halfSize;

            float t = 0.0f;
            if (CheckRayCube(origin, dir, box, t)) {
                if (t <= segmentLength) {
                    rocket.active = false;
                    // printf("Foguete explodiu na Palmeira!\n");
                    AudioExplosionSound();
                    break;
                }
            }
        }
    }
}


void CheckRocketTreeCollision(Kart& shooter, const std::vector<glm::vec3>& positions) {

    // Tamanho da caixa de colisão da árvore
    glm::vec3 halfSize(0.5f, 10.0f, 0.5f);

    for (auto& rocket : shooter.rockets) {
        if (!rocket.active) continue;

        glm::vec3 origin = glm::vec3(rocket.prevPosition);
        glm::vec3 diff = glm::vec3(rocket.position - rocket.prevPosition);
        float segmentLength = length(diff);
        if (segmentLength <= 0.0f) continue;
        glm::vec3 dir = normalize(diff);

        for (const auto& pos : positions) {
            AABB box{};
            glm::vec3 collisionPos = pos;
            collisionPos.y = 0.0f;
            box.min = collisionPos - halfSize;
            box.max = collisionPos + halfSize;

            float t = 0.0f;
            if (CheckRayCube(origin, dir, box, t)) {
                if (t <= segmentLength) {
                    rocket.active = false;
                    // printf("Foguete explodiu na Arvore!\n");
                    AudioExplosionSound();
                    break;
                }
            }
        }
    }
}