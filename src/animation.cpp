#include "animation.h"

// Curva de Bèzier cúbica com os polinômios de Bernstein
glm::vec3 Bezier3(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
{
    float mt = 1.0f - t;
    float mt2 = mt * mt;
    float mt3 = mt2 * mt;
    float t2 = t * t;
    float t3 = t2 * t;

    return mt3 * p0 +
           3.0f * mt2 * t * p1 +
           3.0f * mt * t2 * p2 +
           t3 * p3;
}