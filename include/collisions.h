
#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>
class Kart;
class Coin;
class Scene;

struct BoundingSphere {
    glm::vec3 center;
    float radius;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};


bool CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b);
bool CheckPointSphere(const glm::vec3& point, const BoundingSphere& sphere);
bool CheckSphereCube(const BoundingSphere& sphere, const AABB& box);
bool CheckRayCube(const glm::vec3& origin, const glm::vec3& dir, const AABB& box, float& t);
bool CheckRaySphere(const glm::vec3& origin, const glm::vec3& dir, const BoundingSphere& sphere, float& t);

void HandleCollisions(Scene& scene);

void CheckRocketHits(Kart& shooter, Kart& target);
void CheckKartCoinCollision(Kart& kart, Coin& coin);
void CheckKartKartCollision(Kart& kartA, Kart& kartB);

#endif //COLLISIONS_H
