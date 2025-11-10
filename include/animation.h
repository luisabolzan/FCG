
#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "matrices.h"
#include "globals.h"

glm::vec3 Bezier3(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t);

class Animation {
    private:
    int id;
    
};

#endif