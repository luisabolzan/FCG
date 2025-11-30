#include <algorithm>
#include <cmath>
#include "collisions.h"
#include "kart.h"
#include "coin.h"
#include "scene.h"
#include "audio.h"

// ==================================================================================
// IMPLEMENTAÇÃO DAS PRIMITIVAS DE COLISÃO
// ==================================================================================

// Colisão Kart (sphere) com Kart (sphere)
bool CheckSphereSphere(const BoundingSphere& sphereA, const BoundingSphere& sphereB) {
    // Calcula o vetor entre os centros
    glm::vec3 diff = sphereA.center - sphereB.center;
    float distanceSquared = glm::dot(diff, diff);
    float radiusSum = sphereA.radius + sphereB.radius;

    // Comparamos o quadrado da distância para evitar a raiz quadrada (otimização)
    return distanceSquared <= (radiusSum * radiusSum);
}

// Colisão Coin (ponto) com Kart (sphere)
bool CheckPointSphere(const glm::vec3& point, const BoundingSphere& sphere) {

    glm::vec3 diff = point - sphere.center;
    float distanceSquared = glm::dot(diff, diff);

    return distanceSquared <= (sphere.radius * sphere.radius);
}

// Colisão Kart (sphere) com Cenário (Cube)
bool CheckSphereAABB(const BoundingSphere& sphere, const AABB& box) {
    // Acha o ponto dentro da caixa mais próximo do centro da esfera
    // O 'clamp' prende o valor do centro entre o min e max da caixa
    glm::vec3 closestPointOnBox = glm::clamp(sphere.center, box.min, box.max);

    // Calcula a distância entre esse ponto e o centro da esfera
    glm::vec3 diff = sphere.center - closestPointOnBox;
    float distanceSquared = glm::dot(diff, diff);

    return distanceSquared <= (sphere.radius * sphere.radius);
}

// Colisão rocket (ray) com Cenário (Cube)
bool CheckRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box, float& intersectionDist) {
    // Algoritmo: "Slab Method" (Interseção de planos paralelos)

    // Prevenção de divisão por zero:
    // Se a direção for 0 em algum eixo, usamos um número muito grande para representar infinito
    glm::vec3 invDir;
    invDir.x = (std::abs(rayDir.x) < 1e-6f) ? 1e20f : 1.0f / rayDir.x;
    invDir.y = (std::abs(rayDir.y) < 1e-6f) ? 1e20f : 1.0f / rayDir.y;
    invDir.z = (std::abs(rayDir.z) < 1e-6f) ? 1e20f : 1.0f / rayDir.z;

    glm::vec3 tMinVec = (box.min - rayOrigin) * invDir;
    glm::vec3 tMaxVec = (box.max - rayOrigin) * invDir;

    // Garante que min é menor que max
    glm::vec3 t1 = glm::min(tMinVec, tMaxVec);
    glm::vec3 t2 = glm::max(tMinVec, tMaxVec);

    // O raio entra na caixa quando passa pelo MAIOR dos planos "iniciais" (tNear)
    float tNear = std::max({t1.x, t1.y, t1.z});

    // O raio sai da caixa quando passa pelo MENOR dos planos "finais" (tFar)
    float tFar  = std::min({t2.x, t2.y, t2.z});

    // Se o ponto de entrada for depois do ponto de saída, o raio errou a caixa.
    // Ou se a caixa está atrás do raio (tFar < 0)
    if (tNear > tFar || tFar < 0.0f)
        return false;

    // Se tNear < 0, significa que a origem do raio está DENTRO da caixa
    intersectionDist = (tNear < 0.0f) ? 0.0f : tNear;

    return true;
}

// Colisão rocket (ray) com Kart (sphere)
bool CheckRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingSphere& sphere, float& intersectionDist) {
    // Matemática: Resolvemos |origin + t*dir - center|^2 = radius^2
    // Isso vira uma equação quadrática: at^2 + bt + c = 0

    glm::vec3 originToCenter = rayOrigin - sphere.center;

    float a = glm::dot(rayDir, rayDir); // Geralmente 1.0 se rayDir for normalizado
    float b = 2.0f * glm::dot(originToCenter, rayDir);
    float c = glm::dot(originToCenter, originToCenter) - (sphere.radius * sphere.radius);

    float discriminant = (b * b) - (4.0f * a * c);

    // Se discriminante < 0, o raio errou a esfera
    if (discriminant < 0.0f)
        return false;

    float sqrtDiscriminant = std::sqrt(discriminant);

    // Duas soluções (t1 e t2)
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    // Queremos a interseção positiva mais próxima
    if (t1 >= 0.0f) {
        intersectionDist = t1;
        return true;
    }
    if (t2 >= 0.0f) {
        intersectionDist = t2;
        return true;
    }

    return false;
}


// ==================================================================================
// LÓGICA DE JOGO
// ==================================================================================

// Função auxiliar interna para empurrar o kart para fora
void ResolveSphereAABBCollision(Kart& kart, const BoundingSphere& sphere, const AABB& box) {
    glm::vec3 closestPoint = glm::clamp(sphere.center, box.min, box.max);
    glm::vec3 diff = sphere.center - closestPoint;

    // Evita erro de normalização com vetor zero
    float distSq = glm::dot(diff, diff);
    if (distSq < 1e-6f) return;

    float dist = std::sqrt(distSq);
    float penetration = sphere.radius - dist;

    if (penetration > 0.0f) {
        // Empurra o kart para trás na direção oposta à colisão
        glm::vec4 correction = glm::vec4(glm::normalize(diff) * penetration, 0.0f);
        kart.position += correction;
    }
}


void HandleCollisions(Scene& scene) {
    // Foguetes
    CheckRocketKartCollision(scene.player1, scene.player2);
    CheckRocketKartCollision(scene.player2, scene.player1);

    // Foguetes vs Cenário
    CheckRocketPalmCollision(scene.player1, scene.palmPositions);
    CheckRocketTreeCollision(scene.player1, scene.treePositions);
    CheckRocketPalmCollision(scene.player2, scene.palmPositions);
    CheckRocketTreeCollision(scene.player2, scene.treePositions);

    // Moedas
    for (auto& c : scene.coins) {
        CheckKartCoinCollision(scene.player1, c);
        CheckKartCoinCollision(scene.player2, c);
    }

    // Kart vs Kart
    CheckKartKartCollision(scene.player1, scene.player2);

    // Kart vs Cenário
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

        glm::vec3 rayStart = glm::vec3(rocket.prevPosition);
        glm::vec3 diff = glm::vec3(rocket.position - rocket.prevPosition);
        float segmentLength = glm::length(diff);

        if (segmentLength <= 0.0f) continue;

        glm::vec3 rayDir = glm::normalize(diff);

        BoundingSphere targetSphere{};
        targetSphere.center = glm::vec3(target.position);
        targetSphere.radius = target.radius;

        float hitDistance = 0.0f;
        if (CheckRaySphere(rayStart, rayDir, targetSphere, hitDistance)) {
            // Verifica se o hit ocorreu dentro do segmento percorrido neste frame
            if (hitDistance >= 0.0f && hitDistance <= segmentLength) {
                rocket.active = false;
                target.isAlive = false;
                target.health = 0;
                shooter.score += 100;
                AudioExplosionSound();
                // printf(" %s atingiu %s!\n", shooter.name.c_str(), target.name.c_str());
            }
        }
    }
}

void CheckKartCoinCollision(Kart& kart, Coin& coin) {

    if (!coin.active) return;

    BoundingSphere kartSphere{};
    kartSphere.center = glm::vec3(kart.position);
    kartSphere.radius = kart.radius;

    glm::vec3 coinPos = glm::vec3(coin.position);

    if (CheckPointSphere(coinPos, kartSphere)) {
        coin.active = false;
        coin.respawnTimer = 0.0f;
        kart.ammo++;
        AudioCoinSound();
    }
}

void CheckKartKartCollision(Kart& kartA, Kart& kartB) {

    if (!kartA.isAlive || !kartB.isAlive) return;

    BoundingSphere sphereA{ glm::vec3(kartA.position), kartA.radius };
    BoundingSphere sphereB{ glm::vec3(kartB.position), kartB.radius };

    if (CheckSphereSphere(sphereA, sphereB)) {
        glm::vec3 diff = sphereA.center - sphereB.center;
        float currentDist = glm::length(diff);
        float penetration = (sphereA.radius + sphereB.radius) - currentDist;

        glm::vec3 normal = (currentDist == 0.0f) ? glm::vec3(1,0,0) : glm::normalize(diff);
        glm::vec4 correction = glm::vec4(normal * (penetration * 0.5f), 0.0f);

        kartA.position += correction;
        kartB.position -= correction;
    }
}

void CheckKartPalmCollision(Kart& kart, const std::vector<glm::vec3>& positions) {

    BoundingSphere kartSphere{ glm::vec3(kart.position), kart.radius };
    glm::vec3 halfSize(0.1f, 5.0f, 0.1f); // Raio do tronco da palmeira

    for (const auto& pos : positions) {
        AABB box{ pos - halfSize, pos + halfSize };

        if (CheckSphereAABB(kartSphere, box)) {
            ResolveSphereAABBCollision(kart, kartSphere, box);
            kart.speed = 0.0f;
        }
    }
}

void CheckKartTreeCollision(Kart& kart, const std::vector<glm::vec3>& positions) {

    BoundingSphere kartSphere{ glm::vec3(kart.position), kart.radius };
    glm::vec3 halfSize(0.1f, 10.0f, 0.1f); // Raio do tronco da árvore

    for (const auto& pos : positions) {
        glm::vec3 collisionPos = pos;
        collisionPos.y = 0.0f;
        AABB box{ collisionPos - halfSize, collisionPos + halfSize };

        if (CheckSphereAABB(kartSphere, box)) {
            ResolveSphereAABBCollision(kart, kartSphere, box);
            kart.speed = 0.0f;
        }
    }
}

void CheckRocketPalmCollision(Kart& shooter, const std::vector<glm::vec3>& positions) {

    glm::vec3 halfSize(0.5f, 5.0f, 0.5f); // Hitbox da palmeira para foguete

    for (auto& rocket : shooter.rockets) {
        if (!rocket.active) continue;

        glm::vec3 origin = glm::vec3(rocket.prevPosition);
        glm::vec3 diff = glm::vec3(rocket.position - rocket.prevPosition);
        float len = glm::length(diff);
        if (len <= 0.0f) continue;
        glm::vec3 dir = glm::normalize(diff);

        for (const auto& pos : positions) {
            AABB box{ pos - halfSize, pos + halfSize };
            float hitT = 0.0f;

            if (CheckRayAABB(origin, dir, box, hitT)) {
                if (hitT <= len) {
                    rocket.active = false;
                    AudioExplosionSound();
                    break;
                }
            }
        }
    }
}

void CheckRocketTreeCollision(Kart& shooter, const std::vector<glm::vec3>& positions) {

    glm::vec3 halfSize(0.5f, 10.0f, 0.5f); // Hitbox da árvore para foguete

    for (auto& rocket : shooter.rockets) {
        if (!rocket.active) continue;

        glm::vec3 origin = glm::vec3(rocket.prevPosition);
        glm::vec3 diff = glm::vec3(rocket.position - rocket.prevPosition);
        float len = glm::length(diff);
        if (len <= 0.0f) continue;
        glm::vec3 dir = glm::normalize(diff);

        for (const auto& pos : positions) {
            glm::vec3 collisionPos = pos;
            collisionPos.y = 0.0f;
            AABB box{ collisionPos - halfSize, collisionPos + halfSize };

            float hitT = 0.0f;
            if (CheckRayAABB(origin, dir, box, hitT)) {
                if (hitT <= len) {
                    rocket.active = false;
                    AudioExplosionSound();
                    break;
                }
            }
        }
    }
}