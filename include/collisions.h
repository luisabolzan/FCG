#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>
#include <vector>

// Forward declarations para evitar dependências circulares
class Kart;
class Coin;
class Scene;

// ==================================================================================
// ESTRUTURAS DE DADOS
// ==================================================================================

struct BoundingSphere {
    glm::vec3 center;
    float radius;
};

// AABB: Axis-Aligned Bounding Box
struct AABB {
    glm::vec3 min; // Canto inferior esquerdo traseiro
    glm::vec3 max; // Canto superior direito frontal
};

// ==================================================================================
// PRIMITIVAS DE COLISÃO
// ==================================================================================

/**
 * Verifica se duas esferas estão colidindo.
 * Baseado na distância euclidiana entre os centros.
 */
bool CheckSphereSphere(const BoundingSphere& sphereA, const BoundingSphere& sphereB);

/**
 * Verifica se um ponto específico está dentro de uma esfera.
 */
bool CheckPointSphere(const glm::vec3& point, const BoundingSphere& sphere);

/**
 * Verifica se uma esfera colide com uma AABB (Cubo).
 */
bool CheckSphereAABB(const BoundingSphere& sphere, const AABB& box);

/**
 * Raycasting contra Esfera.
 * Resolve a equação quadrática para encontrar onde o raio intercepta a esfera.
 */
bool CheckRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingSphere& sphere, float& intersectionDist);

/**
 * Raycasting contra AABB (Cubo).
 * Utiliza o "Slab Method" (Kay-Kajiya) para verificar intersecção.
 */
bool CheckRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box, float& intersectionDist);


// ==================================================================================
// LÓGICA DE JOGO
// ==================================================================================

void HandleCollisions(Scene& scene);

void CheckRocketKartCollision(Kart& shooter, Kart& target);
void CheckKartCoinCollision(Kart& kart, Coin& coin);
void CheckKartKartCollision(Kart& kartA, Kart& kartB);

void CheckKartPalmCollision(Kart& kart, const std::vector<glm::vec3>& positions);
void CheckKartTreeCollision(Kart& kart, const std::vector<glm::vec3>& positions);

void CheckRocketPalmCollision(Kart& shooter, const std::vector<glm::vec3>& positions);
void CheckRocketTreeCollision(Kart& shooter, const std::vector<glm::vec3>& positions);

#endif //COLLISIONS_H