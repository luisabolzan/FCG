#include "collisions.h"
#include "kart.h"
#include <algorithm>
#include "coin.h"

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

// Colisão Bala (ray) com Cenário (Cube)
bool CheckRayCube(const glm::vec3& origin, const glm::vec3& dir, const AABB& box, float& t) {
    glm::vec3 t1 = (box.min - origin) / dir;
    glm::vec3 t2 = (box.max - origin) / dir;
    glm::vec3 tmin = glm::min(t1, t2);
    glm::vec3 tmax = glm::max(t1, t2);

    float tNear = std::max({tmin.x, tmin.y, tmin.z});
    float tFar  = std::min({tmax.x, tmax.y, tmax.z});

    if (tNear > tFar || tFar < 0.0f)
        return false;

    t = tNear;
    return true;
}

// Colisão Bala (ray) com Kart (sphere)
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

void CheckRocketHits(Kart& shooter, Kart& target) {
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
            printf(" %s atingiu %s!\n", shooter.name.c_str(), target.name.c_str());
        }
    }
}

void CheckKartCoinCollision(Kart& kart, Coin& coin) {

    if (!coin.active) return;

    BoundingSphere kartSphere;
    kartSphere.center = glm::vec3(kart.position);
    kartSphere.radius = kart.radius;

    glm::vec3 coinPoint = glm::vec3(coin.position);

    if (CheckPointSphere(coinPoint, kartSphere)) {

        coin.active = false;
        coin.respawnTimer = 0.0f;
        kart.ammo++;
        printf("%s pegou uma moeda!\n", kart.name.c_str());
    }
}
